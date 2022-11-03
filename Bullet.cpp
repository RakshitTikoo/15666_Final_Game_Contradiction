#include "Bullet.hpp"
#include "GameState.hpp"

#include <array>

CoreBullet::CoreBullet(glm::vec2 pos, glm::vec2 speed) {
    this->pos = pos;
    this->speed = speed;
}
void CoreBullet::draw(Drawer& drawer) {
	drawer.circle(pos, 0.1f, glm::uvec4(255.f, 255.f, 0.f, 255.f));
}
void CoreBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;

	// TODO: Collide with enemies
}

TurretBullet::TurretBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void TurretBullet::draw(Drawer& drawer) {
	drawer.circle(pos, 0.1f, glm::uvec4(255.f, 0.f, 255.f, 255.f));
}
void TurretBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;

	// TODO: Collide with enemies
}

ShooterBullet::ShooterBullet(glm::vec2 pos, glm::vec2 speed) {
	this->pos = pos;
	this->speed = speed;
}
void ShooterBullet::draw(Drawer& drawer) {
	drawer.circle(this->pos, 0.1f, glm::uvec4(255.f, 127.f, 0.f, 255.f));
}
void ShooterBullet::update(float elapsed, GameState& state) {
	this->pos += this->speed * elapsed;

	// TODO: Collide with player
}
