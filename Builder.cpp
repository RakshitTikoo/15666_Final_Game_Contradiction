#include "Builder.hpp"
#include "Drawer.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include "DrawText.hpp"
#include "Player.hpp"

using namespace std;
using namespace glm;

Builder::Builder() {
}

Builder::Builder(int money) {
    this->remaining_money = money;
    this->TextRenderer = DrawText("NotoSansMono_Condensed-Regular.ttf");
    this->player = Player();
}

bool Builder::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    this->window_min = {0.f, 0.f};
    this->window_max = window_size;
    return controls.handle_event(evt, window_size);
}

Player* Builder::update(float elapsed) {
    { // Mouse stuff
        vec2 mouse_absolute = (controls.mouse_loc - this->window_min) / (this->window_max - this->window_min);
        mouse_absolute.y *= (window_max.y - window_min.y) / (window_max.x - window_min.x);
        auto menu_min_x = get_menu_item_bounds(0).first.x;

        menu_hover = -1;

        if (mouse_absolute.x >= menu_min_x) { // Mouse over menu
            // Check for hover over a menu item
            for (int i = 0; i < PlayerTriangle::NUM_TRIANGLE_TYPES; i++) {
                auto box = get_menu_item_bounds(i);
                if (box.first.x <= mouse_absolute.x && mouse_absolute.x <= box.second.x &&
                    box.first.y <= mouse_absolute.y && mouse_absolute.y <= box.second.y) {
                    menu_hover = i;
                }
            }

            // Check for new selected item
            if (controls.mouse.pressed && menu_hover != -1) {
                menu_selected = menu_hover;
            }
        } else { // Mouse over building area
            // Coordinates of mouse in building space
            vec2 building_space_coords = (controls.mouse_loc - this->window_min) / (this->window_max - this->window_min) * (this->drawer_max - this->drawer_min) + this->drawer_min;

        }
    }

    return nullptr;
}

void Builder::draw(glm::uvec2 const &drawable_size) {
	Drawer drawer(drawable_size, this->TextRenderer);
    float width = 40.f;
    drawer.set_center({0.f, 0.f});
    drawer.set_width(width);
    this->drawer_min = drawer.center - glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);
    this->drawer_max = drawer.center + glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);

    // Draw player
    player.draw(drawer);

    // Draw remaining money
    drawer.text("$" + to_string(remaining_money), {20.f, 680.f}, 0.4f);

    // x from 0 to 1, y from 0 to 1/drawer.aspect
    auto line_absolute = [&](vec2 p0, vec2 p1, uvec4 color={255.f,255.f,255.f,255.f}) {
        vec2 mins = drawer.center - vec2(width/2, (width/drawer.aspect)/2);
        p0 = mins + p0 * vec2(width, width);
        p1 = mins + p1 * vec2(width, width);
        drawer.line(p0, p1, color);
    };

    { // Draw triangle types menu
        // Draw vertical divider line
        auto tmp = get_menu_item_bounds(0);
        float menu_item_sz = tmp.second.x - tmp.first.x;
        float triangle_sz = menu_item_sz * 0.5f;
        line_absolute({1-menu_item_sz, 0}, {1-menu_item_sz, 1.f/drawer.aspect});

        // Draw available triangle types
        for (int i = 0; i < PlayerTriangle::NUM_TRIANGLE_TYPES; i++) {
            auto box = get_menu_item_bounds(i);
            vec2 box_mins = box.first;
            vec2 box_maxs = box.second;
            vec2 mid = (box_mins + box_maxs) / 2.f;

            vec2 p0 = mid + vec2(-triangle_sz/2.f, -sqrtf(3)*triangle_sz/6.f);
            vec2 p1 = mid + vec2( triangle_sz/2.f, -sqrtf(3)*triangle_sz/6.f);
            vec2 p2 = mid + vec2(0.f, sqrtf(3.f)*triangle_sz/3.f);

            glm::uvec4 color = PlayerTriangle::triangleTypeMap[i].color;
            line_absolute(p0, p1, color);
            line_absolute(p1, p2, color);
            line_absolute(p2, p0, color);
        }

        auto draw_selection_box = [&](int idx, uvec4 color) {
            auto box = get_menu_item_bounds(idx);
            float sz = menu_item_sz * 0.8f;
            
            vec2 box_mins = box.first;
            vec2 box_maxs = box.second;
            vec2 mid = (box_mins + box_maxs) / 2.f;

            vec2 p0 = mid + vec2(-sz/2.f, -sz/2.f);
            vec2 p1 = mid + vec2( sz/2.f, -sz/2.f);
            vec2 p2 = mid + vec2( sz/2.f,  sz/2.f);
            vec2 p3 = mid + vec2(-sz/2.f,  sz/2.f);

            line_absolute(p0, p1, color);
            line_absolute(p1, p2, color);
            line_absolute(p2, p3, color);
            line_absolute(p3, p0, color);
        };

        if (menu_hover != -1) {
            // Draw a gray box over hovered menu item
            draw_selection_box(menu_hover, {50.f, 50.f, 50.f, 255.f});

            // Show description of the hovered item in bottom left
            string desc = PlayerTriangle::triangleTypeMap[menu_hover].description;
            drawer.text(desc, {20.f, 20.f}, 0.4f);
        }

        // Draw a white box over selected item
        if (menu_selected != -1) draw_selection_box(menu_selected, {255.f, 255.f, 255.f, 255.f});
    }
}

pair<vec2, vec2> Builder::get_menu_item_bounds(int idx) {
    static float sidelen = 0.05f;
    static float max_x = 1;
    static float max_y = max_x * (window_max.y - window_min.y)/(window_max.x - window_min.x);
    return make_pair(vec2(max_x - sidelen, max_y - idx*sidelen - sidelen),
                     vec2(max_x, max_y - idx*sidelen));
}
