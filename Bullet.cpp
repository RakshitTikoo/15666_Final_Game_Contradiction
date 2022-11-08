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
	this->pos += this->speed * elapsed;
	if (!state.in_arena(this->pos)) {
		destroyed = true;
		return;
	}

	CircleHitbox ourHitbox(this->pos, this->rad);
	std::pair<int,int>* hit = state.player.cluster.intersect(ourHitbox);
	if (hit != nullptr) {
		state.player.destroyTriangle(hit->first, hit->second);
		this->destroyed = true;
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

	//this->angle = glm::acos(glm::dot(init_angle, glm::vec2(1.f, 1.f))/(glm::length(init_angle)*glm::length(glm::vec2(1.f, 1.f))));
	//printf("input angle = %f %f , cos = %f | result = %f \n", init_angle.x, init_angle.y, glm::dot(init_angle, glm::vec2(1.f, 1.f))/(glm::length(init_angle)*glm::length(glm::vec2(1.f, 1.f))) ,this->angle);

}
void SpiralBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, this->color);
}
void SpiralBullet::update(float elapsed, GameState& state) {
	//this->pos += this->speed * elapsed;
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
		this->destroyed = true;
	}
}
