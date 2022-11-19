#include "Enemy.hpp"
#include "GameState.hpp"

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
// TODO: Make a worm tail like linked list
// TODO: The worm element when hit, kill everything below, upto the tail
// TODO: Worm Grows on eating food (1 added at the tail)
Worm::Worm(glm::vec2 pos, Worm *head, Worm *tail, GameState& state) {
    this->pos = pos;
    this->head = head;
    this->tail = tail;

    if(head == nullptr) {
        this->rad = 0.5f;
        // Spawn tail entities
    }
    else {
        this->rad = 0.25f;
    }
}
void Worm::draw(Drawer& drawer) {
    drawer.circle(this->pos, this->rad, this->color);
}
void Worm::update(float elapsed, GameState& state) {
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
Hitbox* Worm::getHitbox() {
    return new CircleHitbox(this->pos, this->rad);
}
