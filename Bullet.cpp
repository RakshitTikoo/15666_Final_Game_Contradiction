#include "Bullet.hpp"
#include "GameState.hpp"
#include "GeoHelpers.hpp"
#include "Hitbox.hpp"
#include <array>

// =====================
// Core Bullet 
// =====================

CoreBullet::CoreBullet(glm::vec2 pos, glm::vec2 speed) {
    this->pos = pos;
    this->speed = speed;
}
void CoreBullet::draw(Drawer& drawer) {
	drawer.circle(pos, this->rad, this->color);
}
void CoreBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	for (Enemy* e : state.enemies) {
		Hitbox* hitbox = e->getHitbox();
		if (ourHitbox.intersect(*hitbox)) {
			e->destroyed = true;
			this->destroyed = true;
		}
	}

	// Hit the Boss
	if (state.trojan != nullptr) {
		std::pair<int,int>* hit = state.trojan->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.trojan->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}

	if (state.infboss != nullptr) {
		std::pair<int,int>* hit = state.infboss->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.infboss->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}

	if (state.timestopboss != nullptr) {
		std::pair<int,int>* hit = state.timestopboss->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.timestopboss->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}


}

// =====================
// Turret Bullet 
// =====================

TurretBullet::TurretBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void TurretBullet::draw(Drawer& drawer) {
	drawer.circle(pos, this->rad, this->color);
}
void TurretBullet::update(float elapsed, GameState& state) {
	this->lifespan -= elapsed;
	if (this->lifespan <= 0) this->destroyed = true;
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	for (Enemy* e : state.enemies) {
		Hitbox* hitbox = e->getHitbox();
		if (ourHitbox.intersect(*hitbox)) {
			e->destroyed = true;
			this->destroyed = true;
		}
	}

	// Hit the Boss
	if (state.trojan != nullptr) {
		std::pair<int,int>* hit = state.trojan->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.trojan->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}

	if (state.infboss != nullptr) {
		std::pair<int,int>* hit = state.infboss->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.infboss->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}

	if (state.timestopboss != nullptr) {
		std::pair<int,int>* hit = state.timestopboss->cluster.intersect(ourHitbox);
		if (hit != nullptr) {
			state.timestopboss->destroyTriangle(hit->first, hit->second);
			this->destroyed = true;
		}
	}
}

// =====================
// Shooter Bullet 
// =====================

ShooterBullet::ShooterBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void ShooterBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void ShooterBullet::update(float elapsed, GameState& state) {
	this->lifespan -= elapsed;
	if (this->lifespan <= 0) this->destroyed = true;
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}

// =====================
// Spiral Bullet 
// =====================
SpiralBullet::SpiralBullet(glm::vec2 pos, glm::vec2 init_angle, float speed) {
	this->pos = pos;
	this->init_angle = init_angle;
	this->speed = speed;
	this->ori_pos = pos;


	if(init_angle.x == 1.f && init_angle.y == 0.f) this->angle = 0.0f;
	if(init_angle.x == 0.f && init_angle.y == 1.f) this->angle = PI/2.f;
	if(init_angle.x == -1.f && init_angle.y == 0.f) this->angle = PI;
	if(init_angle.x == 0.f && init_angle.y == -1.f) this->angle = 3.f*PI/2.f;
}
void SpiralBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void SpiralBullet::update(float elapsed, GameState& state) {
	this->lifespan -= elapsed;
	if (this->lifespan <= 0) this->destroyed = true;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	// Bullet rotation logic	
	rot_rad += speed*elapsed*radial_speed;
	this->pos.x = this->ori_pos.x + rot_rad*std::sin(angle);
	this->pos.y = this->ori_pos.y + rot_rad*std::cos(angle);
	angle +=  speed*elapsed*angular_speed;
	


	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}

// =====================
// Trojan Bullet 
// =====================
TrojanBullet::TrojanBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void TrojanBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void TrojanBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}

// =====================
// Infboss Bullet 
// =====================
InfbossBullet::InfbossBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void InfbossBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void InfbossBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}


// =====================
// Spiral Bullet 
// =====================
InfbossSpiralBullet::InfbossSpiralBullet(glm::vec2 pos, glm::vec2 init_angle, float speed) {
	this->pos = pos;
	this->init_angle = init_angle;
	this->speed = speed;
	this->ori_pos = pos;

	// size angles
	if(init_angle.x == 0.f && init_angle.y == 0.f) this->angle = 0.0f;
	if(init_angle.x == 0.f && init_angle.y == 1.f) this->angle = PI/3.f;
	if(init_angle.x == 1.f && init_angle.y == 0.f) this->angle = 2.f*PI/3.f;
	if(init_angle.x == 1.f && init_angle.y == 1.f) this->angle = 3.f*PI/3.f;
	if(init_angle.x == 0.f && init_angle.y == -1.f) this->angle = 4.f*PI/3.f;
	if(init_angle.x == 1.f && init_angle.y == -1.f) this->angle = 5.f*PI/3.f;
}
void InfbossSpiralBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void InfbossSpiralBullet::update(float elapsed, GameState& state) {
	this->lifespan -= elapsed;
	if (this->lifespan <= 0) this->destroyed = true;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	// Bullet rotation logic	
	rot_rad += speed*elapsed*radial_speed;
	this->pos.x = this->ori_pos.x + rot_rad*std::sin(angle);
	this->pos.y = this->ori_pos.y + rot_rad*std::cos(angle);
	angle +=  speed*elapsed*angular_speed;
	


	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}

// =====================
// Timestop Bullet 
// =====================
TimestopBullet::TimestopBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void TimestopBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void TimestopBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		Sound::play(*state.player_hit, state.sound_effect_volume*2.f, 0.0f);
		this->destroyed = true;
	}

	// Explosion hit logic 
	if(state.in_arena(this->pos))
		if(state.player.explosion_intersect(ourHitbox)) this->destroyed = true;

	// Time stop hit logic 
    if(state.in_arena(this->pos))
        if(state.player.timestop_intersect(ourHitbox)  && this->timestop_hit == false) {
            this->timestop_hit = true;
            this->timestop_hit_cnt = this-> timestop_hit_cooldown;
            this->speed /= 10.f; 
        }
    
    if(this->timestop_hit) {
        this->timestop_hit_cnt -= elapsed;
        if(this->timestop_hit_cnt <= 0.f) {
            this->timestop_hit = false;
            this->speed *= 10.f; 
        }
    }

}