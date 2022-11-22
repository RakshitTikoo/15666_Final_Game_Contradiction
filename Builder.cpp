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
    float width = 40.f;
    drawer.set_center({20.f, 20.f});
    drawer.set_width(40.f);

    // Draw remaining money
    drawer.text(to_string(remaining_money) + " coins", {20.f, 680.f}, 0.4f);

    // x from 0 to 1, y from 0 to 1/drawer.aspect
    auto line_absolute = [&](vec2 p0, vec2 p1, uvec4 color={255.f,255.f,255.f,255.f}) {
        vec2 mins = drawer.center - vec2(width/2, (width/drawer.aspect)/2);
        p0 = mins + p0 * vec2(width, width);
        p1 = mins + p1 * vec2(width, width);
        drawer.line(p0, p1, color);
    };

    // Draw triangle types menu
    float sz = 0.03f;
    line_absolute({1-sz*2.f, 0}, {1-sz*2.f, 1.f/drawer.aspect}); // vertical division
    for (int i = 0; i < PlayerTriangle::NUM_TRIANGLE_TYPES; i++) {
        glm::vec2 p0 = {1 - sz*1.5f, 1.f/drawer.aspect - sz*1.2f - i*sz*1.5f};
        glm::vec2 p1 = p0 + vec2(sz, 0.f);
        glm::vec2 p2 = p0 + vec2(sz/2.f, sz/2.f*sqrtf(3.f));
        glm::uvec4 color = PlayerTriangle::triangleTypeMap[i].color;
        line_absolute(p0, p1, color);
        line_absolute(p1, p2, color);
        line_absolute(p2, p0, color);
    }
}
