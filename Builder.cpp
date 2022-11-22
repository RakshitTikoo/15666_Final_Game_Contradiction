#include "Builder.hpp"
#include "Drawer.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include "DrawText.hpp"
#include "Player.hpp"

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
    drawer.set_center({0.f, 0.f});
    drawer.set_width(40.f);

    // Draw remaining money
    drawer.text(to_string(remaining_money), {20.f, 680.f}, 0.4f);

    // Draw triangle types menu
    for (int i = 0; i < PlayerTriangle::NUM_TRIANGLE_TYPES; i++) {
        glm::vec2 p0 = {18.5f, 20.f/drawer.aspect-1.5f - i * 1.5};
        glm::vec2 p1 = p0 + vec2(1.f, 0.f);
        glm::vec2 p2 = p0 + vec2(0.5f, 0.5f*sqrtf(3.f));
        glm::uvec4 color = PlayerTriangle::triangleTypeMap[i].color;
        drawer.line(p0, p1, color);
        drawer.line(p1, p2, color);
        drawer.line(p2, p0, color);
    }
}
