#include "Enemy.hpp"
#include "GameState.hpp"
#include "rng.hpp"

// ================== 
// CHASER
// ================== 
Chaser::Chaser(glm::vec2 pos) {
    this->pos = pos;
}
void Chaser::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void Chaser::update(float elapsed, GameState& state) {
    // Walk towards player
    glm::vec2 dir = glm::normalize(state.player.cluster.pos - this->pos);
    this->pos += dir * elapsed * mov_speed;

	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

    // Explosion hit logic 
    if(state.in_arena(this->pos))
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;

}
Hitbox* Chaser::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}

// ================== 
// SHOOTER
// ================== 
Shooter::Shooter(glm::vec2 pos) {
    this->pos = pos;
}
void Shooter::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void Shooter::update(float elapsed, GameState& state) {
    // Walk towards player if too far
    glm::vec2 player_pos = state.player.cluster.pos;
    float dist = glm::length(player_pos - this->pos);
    if (dist >= 5.0f) {
        glm::vec2 dir = glm::normalize(player_pos - this->pos);
        this->pos += dir * elapsed * mov_speed;
    }

    // Shoot bullet towards player
    bullet_cooldown -= elapsed;
    if (bullet_cooldown <= 0.0f) {
        ShooterBullet* b = new ShooterBullet(this->pos,  bullet_speed * glm::normalize(player_pos - this->pos));
        state.bullets.push_back(b);
        bullet_cooldown = 1.f;
    }

	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

    // Explosion hit logic 
    if(state.in_arena(this->pos))
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;

}
Hitbox* Shooter::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}

// ================== 
// SPIRAL
// ================== 

Spiral::Spiral(glm::vec2 pos) {
    this->pos = pos;
}
void Spiral::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void Spiral::update(float elapsed, GameState& state) {
    // Walk towards player if too far
    glm::vec2 player_pos = state.player.cluster.pos;
    float dist = glm::length(player_pos - this->pos);
    if (dist >= 5.0f) {
        glm::vec2 dir = glm::normalize(player_pos - this->pos);
        this->pos += dir * elapsed * mov_speed;
    }

    // Shoot bullet towards player
    bullet_cooldown -= elapsed;
    bullet_per_shot_cooldown -= elapsed;

    if (bullet_cooldown <= 0.0f) {
        // Update Logic
        // Spawn 20 bullets from 
        if(bullet_per_shot_cooldown <= 0.0f) {
            SpiralBullet* b_left = new SpiralBullet(this->pos,   glm::vec2(-1.0f, 0.0f), bullet_speed);
            state.bullets.push_back(b_left);

            SpiralBullet* b_right= new SpiralBullet(this->pos, glm::vec2(1.0f, 0.0f), bullet_speed);
            state.bullets.push_back(b_right);

            SpiralBullet* b_up = new SpiralBullet(this->pos,   glm::vec2(0.0f, 1.0f), bullet_speed);
            state.bullets.push_back(b_up);

            SpiralBullet* b_down = new SpiralBullet(this->pos,   glm::vec2(0.0f, -1.0f), bullet_speed);
            state.bullets.push_back(b_down);

            bullet_per_shot_cooldown = 0.25f;

            total_bullets_shot += 1;
        }
        if(total_bullets_shot >= total_bullets_to_shoot) {
            bullet_cooldown = 5.f;
            total_bullets_shot = 0;
        }
            
    }

	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

    // Explosion hit logic
    if(state.in_arena(this->pos)) 
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;
}
Hitbox* Spiral::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}


// ================== 
// BOMBER
// ================== 

Bomber::Bomber(glm::vec2 pos) {
    this->pos = pos;
}
void Bomber::draw(Drawer& drawer) {
    if(blink_render == 0)
        drawer.circle(this->pos, this->rad, this->color);

}
void Bomber::update(float elapsed, GameState& state) {
    // Walk towards player if too far
    glm::vec2 player_pos = state.player.cluster.pos;
    float dist = glm::length(player_pos - this->pos);
    if (dist >= 5.0f) {
        if(bomb_phase == 0){
        glm::vec2 dir = glm::normalize(player_pos - this->pos);
        this->pos += dir * elapsed * mov_speed;
        }
    }
    else {
        bomb_phase = 1;
    }

    if(bomb_phase == 1) {
        if(blink_time > 0) {
            blink_time -= 1;
            if(blink_time % 4 == 0){
                if(blink_render == 0) blink_render = 1;
                else blink_render = 0;
            }
        }
        else bomb_phase = 2;
    }

    if(bomb_phase == 2) {
        blink_render = 0;
        this->rad += mov_speed/2.f;

        if(this->rad >= bomb_rad) {
            this->destroyed = true;
        }
    }

	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
        if(bomb_phase != 2)
		    this->destroyed = true;
	}

    // Explosion hit logic 
    if(state.in_arena(this->pos))
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;

}
Hitbox* Bomber::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}


// ================== 
// INFECTOR
// ================== 
Infector::Infector(glm::vec2 pos) {
    this->pos = pos;
}
void Infector::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void Infector::update(float elapsed, GameState& state) {
    // Walk towards player
    glm::vec2 dir = glm::normalize(state.player.cluster.pos - this->pos);
    float dist = glm::length(state.player.cluster.pos - this->pos);
    if (dist >= 2.0f) {
        this->pos += dir * elapsed * mov_speed;
    }
    else {
        this->pos += dir * elapsed * mov_speed/10.f; // Slower movement when closer
    }
	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
        // Do not destroy, but spawn another infector
        state.enemies.push_back(new Infector(state.player.cluster.getTrianglePosition(hit->first, hit->second)));
        //this->destroyed = true;
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		
	}

    // Explosion hit logic 
    if(state.in_arena(this->pos))
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;

}
Hitbox* Infector::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}

// ================== 
// WORM
// ================== 
WormSegment::WormSegment(glm::vec2 pos, WormSegment *follow) {
    this->pos = pos;
    this->follow = follow;
    float wander_angle = rng::randab(0, 2*acosf(-1.f));
    this->wander_dir = {cosf(wander_angle), sinf(wander_angle)};
    if (follow != nullptr) {
        follow->follower = this;
    }
}
void WormSegment::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void WormSegment::update(float elapsed, GameState& state) {
    // if follow is destroyed, we should be a new worm
    if (this->follow != nullptr && this->follow->destroyed) {
        this->follow = nullptr;
    }
    // also take care of case where follower got destroyed
    if (this->follower != nullptr && this->follower->destroyed) {
        this->follower = nullptr;
    }

    // movement
    if(this->follow == nullptr) { // If this is head
        // Update wander/follow state
        this->wander_timer -= elapsed;
        if (this->wander_timer < 0) {
            if (!this->wandering) { // switch to wander
                this->wander_timer = rng::randab(1.f, 2.f);
                float wander_angle = rng::randab(0, 2*acosf(-1.f));
                this->wander_dir = {cosf(wander_angle), sinf(wander_angle)};
                
                // grow a segment if there's not too many enemies already
                if ((int)state.enemies.size() < 200) {
                    // first, find the last segment in our chain
                    WormSegment* cur = this;
                    while (cur->follower != nullptr) {
                        cur = cur->follower;
                    }
                    // next, add a follower to it
                    // try to add it so there's 3 collinearly, otherwise random
                    float angle = rng::randab(0, 2*acosf(-1.f));
                    glm::vec2 dir = {cosf(angle), sinf(angle)};
                    if (cur->follow != nullptr) {
                        dir = glm::normalize(cur->pos - cur->follow->pos);
                    }
                    glm::vec2 new_pos = cur->pos + dir;
                    state.enemies.push_back(new WormSegment(new_pos, cur));
                }
            } else { // switch to follow
                this->wander_timer = rng::randab(2.f, 3.f);
            }
            this->wandering = !this->wandering;
        }

        if (!this->wandering) {
            // Follow player
            glm::vec2 dir = glm::normalize(state.player.cluster.pos - this->pos);
            this->pos += dir * elapsed * mov_speed;
        } else {
            // Wander
            this->pos += wander_dir * elapsed * mov_speed;
        }
    } else {
        // follow the segment in front
        glm::vec2 towards = glm::normalize(this->pos - follow->pos);
        this->pos = follow->pos + towards * (follow->rad + this->rad);
    }

    // Collide with player
	std::pair<int,int>* hit = state.player.cluster.intersect(*getHitbox());
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
        Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
        this->destroyed = true;
	}

    // Explosion hit logic 
    if(state.in_arena(this->pos))
	    if(state.player.explosion_intersect(*getHitbox())) this->destroyed = true;
}
Hitbox* WormSegment::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}
