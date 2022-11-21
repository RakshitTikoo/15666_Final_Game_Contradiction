#pragma once

#include "Player.hpp"
#include <functional>
#include <vector>
#include "GameState.hpp"
#include "GeoHelpers.hpp"
#include "Sound.hpp"

PlayerTriangle::PlayerTriangle() {
    this->type = CORE;
    this->health = triangle_health[0];
}

PlayerTriangle::PlayerTriangle(int type) {
    this->type = type;
    this->health = triangle_health[type];
}

Player::Player() {
    cluster = TriangleCluster();
	addTriangle(0, 0, PlayerTriangle(0));
    cluster.pos = glm::vec2(0.f, 0.f);
}

void Player::draw(Drawer& drawer) {
    for (std::pair<int,int> coords : cluster.triangles) {
        std::vector<glm::vec2> corners = cluster.getTriangleCorners(coords.first, coords.second);
        PlayerTriangle t = triangle_info[coords];

        //drawer.line(corners[0], corners[1], t.color[t.type]);
        //drawer.line(corners[1], corners[2], t.color[t.type]);
        //drawer.line(corners[2], corners[0], t.color[t.type]);

        // Unnecessary time waste code to draw smaller triangle 
        glm::vec2 offset_0 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[0]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[0]);
        offset_0 = offset_0*0.05f;
        glm::vec2 offset_1 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[1]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[1]);
        offset_1 = offset_1*0.05f;
        glm::vec2 offset_2 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[2]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[2]);
        offset_2 = offset_2*0.05f;

        drawer.line(corners[0] + offset_0 , corners[1] + offset_1, t.color[t.type]);
        drawer.line(corners[1] + offset_1, corners[2] + offset_2, t.color[t.type]);
        drawer.line(corners[2] + offset_2, corners[0] + offset_0, t.color[t.type]);
    }

    if (draw_bbox) {
        glm::uvec4 white = {255.f, 255.f, 255.f, 255.f};
        std::vector<glm::vec2> p = cluster.get_bbox_corners();
        drawer.line(p[0], p[1], white);
        drawer.line(p[1], p[2], white);
        drawer.line(p[2], p[3], white);
        drawer.line(p[3], p[0], white);
    }
}

void Player::draw_explosion(Drawer& drawer) {
    for (int i = 0 ; i < (int)explosion_pos.size(); i++) {
        drawer.circle(explosion_pos[i], explosion_rad[i], glm::uvec4(255.f, 255.f, 0.f, 255.f));
        explosion_rad[i] += explosion_speed;
        if (explosion_rad[i] >= explosion_max_rad[i]) {
            explosion_pos.erase(explosion_pos.begin() + i);
            explosion_rad.erase(explosion_rad.begin() + i);
            explosion_max_rad.erase(explosion_max_rad.begin() + i);
        }
    }
}

bool Player::explosion_intersect(const Hitbox& hitbox) {
    for (int i = 0 ; i < (int)explosion_pos.size(); i++) {
        CircleHitbox explosion_hitbox = CircleHitbox(explosion_pos[i], explosion_rad[i]);
        if (explosion_hitbox.intersect(hitbox)) return true;
    }
    return false;
}

void Player::update(float elapsed, GameState& gs, Controls& controls) {
    // ===================
    // player movement
    // ===================
    {
        // combine inputs into a move:
        glm::vec2 move = glm::vec2(0.0f);
        if (controls.left.pressed && !controls.right.pressed) move.x =-1.0f;
        if (!controls.left.pressed && controls.right.pressed) move.x = 1.0f;
        if (controls.down.pressed && !controls.up.pressed) move.y =-1.0f;
        if (!controls.down.pressed && controls.up.pressed) move.y = 1.0f;
        // make it so that moving diagonally doesn't go faster:
        if (move != glm::vec2(0.0f)) move = player_speed * glm::normalize(move) * elapsed;

        // Clip player inside arena
        cluster.pos += move;
        cluster.pos = max(gs.arena_min, cluster.pos);
        cluster.pos = min(gs.arena_max, cluster.pos);

        if (controls.q.pressed) {
            cluster.angle -= player_rot * elapsed;
            cluster.angle = std::fmodf(cluster.angle + 360.0f, 360.0f);
        }
        if (controls.e.pressed) {
            cluster.angle += player_rot * elapsed;
            cluster.angle = std::fmodf(cluster.angle + 360.0f, 360.0f);
        }
    }

    // ==============================
    // eat food = grow a triangle
    // ==============================
    {
        std::vector<int> toErase;
        std::vector<std::pair<std::pair<int, int>, PlayerTriangle>> toInsert;
        for (int i = 0; i < (int)gs.food.size(); i++) {
            glm::vec2 foodpos = gs.food[i];
            for (std::pair<int,int> coords : cluster.triangles) {
                std::vector<glm::vec2> corners = cluster.getTriangleCorners(coords.first, coords.second);

                // is foodpos inside the triangle?
                if (GeoHelpers::pointInTriangle(foodpos, corners[0], corners[1], corners[2])) {
                    toErase.push_back(i);
                    // play sound
                    Sound::play(*gs.player_grow, gs.sound_effect_volume*0.5f, 0.0f);

                    // add a new triangle to the nearest side
                    float d1 = GeoHelpers::pointToSegmentDistance(foodpos, corners[0], corners[1]);
                    float d2 = GeoHelpers::pointToSegmentDistance(foodpos, corners[1], corners[2]);
                    float d3 = GeoHelpers::pointToSegmentDistance(foodpos, corners[2], corners[0]);

                    float minDist = fmin(d1, fmin(d2, d3));

                    auto addTriangle = [&](int x, int y) {
                        //int curr_type = ; 
                        toInsert.push_back({{x, y}, PlayerTriangle(std::rand()%4 + 1)});
                    };

                    if (minDist == d1) {
                        if (coords.first%2 == 0) {
                            addTriangle(coords.first+1, coords.second-1);
                        } else {
                            addTriangle(coords.first-1, coords.second+1);
                        }
                    } else if (minDist == d2) {
                        if (coords.first%2 == 0) {
                            addTriangle(coords.first+1, coords.second);
                        } else {
                            addTriangle(coords.first-1, coords.second);
                        }
                    } else {
                        if (coords.first%2 == 0) {
                            addTriangle(coords.first-1, coords.second);
                        } else {
                            addTriangle(coords.first+1, coords.second);
                        }
                    }
                    break;
                }
            }
        }
        for (int i = (int)toErase.size()-1; i >= 0; i--) {
            gs.food.erase(gs.food.begin() + toErase[i]);
        }
        for (auto k : toInsert) {
            std::pair<int,int> coords = k.first;
            PlayerTriangle t = k.second;
            if (!cluster.triangles.count({coords.first, coords.second})) {
                addTriangle(coords.first, coords.second, t);
            }
        }
    }

    // =================
    // player shooting
    // =================
    {
        time_since_shoot += elapsed;
        for (auto& k : triangle_info) if (k.second.type == 2) {
            k.second.time_since_shoot += elapsed;
        }

        if (controls.mouse.pressed) {
            if (time_since_shoot >= SHOOT_COOLDOWN) {
                time_since_shoot = 0.f;
                glm::vec2 dir = glm::normalize(controls.mouse_loc - cluster.pos);
                gs.bullets.push_back(new CoreBullet(cluster.pos, core_bullet_speed * dir));
                Sound::play(*gs.player_bullet, gs.sound_effect_volume*0.5f, 0.0f);
            }
            for (auto& k : triangle_info) if (k.second.type == 2) {
                std::pair<int, int> coords = k.first;
                PlayerTriangle& t = k.second;
                if (t.time_since_shoot >= t.SHOOT_COOLDOWN) {
                    t.time_since_shoot = 0.f;
                    glm::vec2 p = cluster.getTrianglePosition(coords.first, coords.second);
                    glm::vec2 dir = glm::normalize(controls.mouse_loc - p);
                    gs.bullets.push_back(new TurretBullet(p, turret_bullet_speed * dir));
                    Sound::play(*gs.player_bullet, gs.sound_effect_volume*0.3f, 0.0f);
                }
            }
        }
    }


    // =====================
    // player bomb attack
    // =====================
    if(controls.space.pressed && controls.space.once == 1) {
        controls.space.once = 2; // One Action per Press
        //printf("Bomb Attack \n");
        explosion_pos.push_back(cluster.pos);
        explosion_max_rad.push_back(std::min((float)cluster.triangles.size() - 1.f, 10.f));
        explosion_rad.push_back(0.0f);

        // Destroy Triangles
        for (auto& k : triangle_info)  {
            std::pair<int, int> coords = k.first;

            if(!(coords.first == 0 && coords.second == 0)) {
                if(cluster.triangles.count({coords.first, coords.second})) { // Check if triagle in cluster
                    //triangle_info[{coords.first, coords.second}] = 1; // Reset Hit Box
                    destroyTriangle(coords.first, coords.second);
                }    
            }
        
        }
    }

}

void Player::addTriangle(int i, int j, PlayerTriangle t) {
    assert(!cluster.triangles.count({i, j}));
    cluster.insertTriangle(i, j);
    triangle_info[{i,j}] = t;
}

void Player::destroyTriangle(int i, int j) {
    assert(cluster.triangles.count({i, j}));
    triangle_info[{i, j}].health -= 1;
    bool inf_fl = (triangle_info[{i, j}].type == PlayerTriangle::INFECTOR);
    if(triangle_info[{i, j}].health <= 0)
    {
        eraseSingleTriangle(i, j);
        dfsEraseTriangles();
    }
    if(inf_fl) {
        addTriangle(i, j, PlayerTriangle(PlayerTriangle::BASIC));
        // Randomly add a triangle 
        std::pair<int, int> next_triangle[3];
        if(i % 2 == 0)
        {
            // (i+1,j), (i-1,j), and (i+1,j-1)
            next_triangle[0] = {i+1,j};
            next_triangle[1] = {i-1,j};
            next_triangle[2] = {i+1,j-1};
        }
        else {
            // (i+1,j), (i-1,j), and (i-1,j+1)
            next_triangle[0] = {i+1,j};
            next_triangle[1] = {i-1,j};
            next_triangle[2] = {i-1,j+1};
        }
        int selection = std::rand() % 3;
        while(triangle_info.find(next_triangle[selection]) != triangle_info.end()) {
          // found
          selection += 1;
          if(selection >= 3) selection = 0;
        } 
        
        addTriangle(next_triangle[selection].first, next_triangle[selection].second, PlayerTriangle(PlayerTriangle::BASIC));
    }
}

void Player::destroyTriangles(std::vector<std::pair<int,int>> coords) {
    for (std::pair<int,int> c : coords) {
        eraseSingleTriangle(c.first, c.second);
    }
    dfsEraseTriangles();
}

void Player::eraseSingleTriangle(int i, int j) {
    assert(cluster.triangles.count({i, j}));
    cluster.eraseTriangle(i, j);
    triangle_info.erase({i, j});
}

void Player::dfsEraseTriangles() {
    std::function<void(int, int, std::set<std::pair<int, int>>)> do_dfs;
    std::set<std::pair<int, int>> visited;
    dfsEraseTrianglesInner(0, 0, visited);
    
    // delete triangles that are not visited
    std::vector<std::pair<int, int>> toErase;
    for (std::pair<int, int> t : cluster.triangles) {
        if (!visited.count(t)) toErase.push_back(t);
    }
    for (std::pair<int, int> t : toErase) eraseSingleTriangle(t.first, t.second);
}

void Player::dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited) {
    if (!cluster.triangles.count({i, j})) return;
    if (visited.count({i, j})) return;
    
    visited.insert({i, j});

    if (i % 2 == 0) {
        dfsEraseTrianglesInner(i + 1, j, visited);
        dfsEraseTrianglesInner(i + 1, j - 1, visited);
        dfsEraseTrianglesInner(i - 1, j, visited);
    } else {
        dfsEraseTrianglesInner(i + 1, j, visited);
        dfsEraseTrianglesInner(i - 1, j + 1, visited);
        dfsEraseTrianglesInner(i - 1, j, visited);
    }
}
