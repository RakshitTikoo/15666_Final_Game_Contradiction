#include "Builder.hpp"
#include "Drawer.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include "DrawText.hpp"

using namespace std;
using namespace glm;

Builder::Builder() {
    return;
}

Builder::Builder(int money) {
    this->remaining_money = money;
}

bool Builder::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    return false;
}

Player* Builder::update(float elapsed) {
    return nullptr;
}

void Builder::draw(glm::uvec2 const &drawable_size) {
	Drawer drawer(drawable_size);
    drawer.text("yoyoyooyoyoyo", vec2(-0.4f, 0.1f), 0.05f);
    return;
}
