#include "Bullet.hpp"
#include "GameState.hpp"
#include "GeoHelpers.hpp"
#include "Hitbox.hpp"
#include <array>

CoreBullet::CoreBullet(glm::vec2 pos, glm::vec2 speed) {
    this->pos = pos;
    this->speed = speed;
}
void CoreBullet::draw(Drawer& drawer) {
	drawer.circle(pos, this->rad, glm::uvec4(255.f, 255.f, 0.f, 255.f));
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

TurretBullet::TurretBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void TurretBullet::draw(Drawer& drawer) {
	drawer.circle(pos, this->rad, glm::uvec4(255.f, 0.f, 255.f, 255.f));
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

ShooterBullet::ShooterBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void ShooterBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, this->rad, glm::uvec4(255.f, 127.f, 0.f, 255.f));
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
