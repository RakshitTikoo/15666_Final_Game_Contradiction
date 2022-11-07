#pragma once

#include "Player.hpp"
#include <functional>
#include <vector>
#include "GameState.hpp"
#include "GeoHelpers.hpp"
#include "Sound.hpp"

PlayerTriangle::PlayerTriangle() {
    this->type = 0;
}

PlayerTriangle::PlayerTriangle(int type) {
    this->type = type;
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

        drawer.line(corners[0], corners[1], t.color[t.type]);
        drawer.line(corners[1], corners[2], t.color[t.type]);
        drawer.line(corners[2], corners[0], t.color[t.type]);
    }
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
                        toInsert.push_back({{x, y}, PlayerTriangle(std::rand()%2 + 1)});
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

    // player shooting
    {
        time_since_shoot += elapsed;
        for (auto& k : triangle_info) if (k.second.type == 2) {
            k.second.time_since_shoot += elapsed;
        }

        if (controls.mouse.pressed) {
            if (time_since_shoot >= SHOOT_COOLDOWN) {
                time_since_shoot = 0.f;
                glm::vec2 dir = glm::normalize(controls.mouse_loc - cluster.pos);
                gs.bullets.push_back(new CoreBullet(cluster.pos, 20.f * dir));
                Sound::play(*gs.player_bullet, gs.sound_effect_volume*0.5f, 0.0f);
            }
            for (auto& k : triangle_info) if (k.second.type == 2) {
                std::pair<int, int> coords = k.first;
                PlayerTriangle& t = k.second;
                if (t.time_since_shoot >= t.SHOOT_COOLDOWN) {
                    t.time_since_shoot = 0.f;
                    glm::vec2 p = cluster.getTrianglePosition(coords.first, coords.second);
                    glm::vec2 dir = glm::normalize(controls.mouse_loc - p);
                    gs.bullets.push_back(new TurretBullet(p, 8.f * dir));
                    Sound::play(*gs.player_bullet, gs.sound_effect_volume*0.3f, 0.0f);
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
    eraseSingleTriangle(i, j);
    dfsEraseTriangles();
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
