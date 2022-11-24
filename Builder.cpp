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

        done_hovered = false;
        building_hovered = false;
        menu_hover = -1;

        
        { // Check for hover over done button
            auto box = get_finished_button_bounds();
            if (box.first.x <= mouse_absolute.x && mouse_absolute.x <= box.second.x &&
                box.first.y <= mouse_absolute.y && mouse_absolute.y <= box.second.y) {
                done_hovered = true;
                if (controls.mouse.pressed) {
                    return &player;
                }
            }
        }
        
        if (!done_hovered) {
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
                building_hovered = true;
                building_hover = get_triangle_coords(building_space_coords);

                // Try to place a triangle
                bool contains_triangle = player.triangle_info.count({building_hover.first, building_hover.second});
                if (controls.mouse.pressed) {
                    if (!contains_triangle && menu_selected >= 1) {
                        player.addTriangle(building_hover.first, building_hover.second, menu_selected);
                    } else if (contains_triangle && menu_selected == 0 && building_hover != make_pair(0, 0)) {
                        player.eraseSingleTriangle(building_hover.first, building_hover.second);
                    }
                }
            }
        }
    }

    { // Moving around
        glm::vec2 move = glm::vec2(0.0f);
        if (controls.left.pressed && !controls.right.pressed) move.x =-1.0f;
        if (!controls.left.pressed && controls.right.pressed) move.x = 1.0f;
        if (controls.down.pressed && !controls.up.pressed) move.y =-1.0f;
        if (!controls.down.pressed && controls.up.pressed) move.y = 1.0f;
        // make it so that moving diagonally doesn't go faster:
        if (move != glm::vec2(0.0f)) move = 10.f * glm::normalize(move) * elapsed;
        center += move;
    }

    { // Check if the structure is disconnected
        disconnected = player.structureDisconnected();
    }

    return nullptr;
}

void Builder::draw(glm::uvec2 const &drawable_size) {
	Drawer drawer(drawable_size, this->TextRenderer);
    float width = 40.f;
    drawer.set_center(center);
    drawer.set_width(width);
    this->drawer_min = drawer.center - glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);
    this->drawer_max = drawer.center + glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);

    // Draw lattice dots
    {
        pair<int, int> tmp = get_triangle_coords(drawer.center + vec2(-drawer.width/2 - 1, drawer.width/(2*drawer.aspect) + 1));
        int minx = tmp.first;
        int maxy = tmp.second;
        tmp = get_triangle_coords(drawer.center + vec2(drawer.width/2 + 1, -drawer.width/(2*drawer.aspect)-1));
        int maxx = tmp.first;
        int miny = tmp.second;
        for (int i = minx; i <= maxx; i++) {
            for (int j = miny; j <= maxy; j++) {
                if (i%2 == 0) {
                    vec2 c = player.cluster.getTriangleCorners(i, j)[0];
                    drawer.circle(c, 0.05f, {55.f, 55.f, 55.f, 255.f});
                }
            }
        }
    }

    // Draw player
    player.draw(drawer);

    // Draw remaining money
    drawer.text("$" + to_string(remaining_money), {10.f, window_max.y-30.f}, 0.4f);

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

        // Draw available triangle types
        for (int i = 0; i < PlayerTriangle::NUM_TRIANGLE_TYPES; i++) {
            auto box = get_menu_item_bounds(i);
            vec2 box_mins = box.first;
            vec2 box_maxs = box.second;
            vec2 mid = (box_mins + box_maxs) / 2.f;

            if (i == 0) { // Instead of core, draw eraser icon
                float cross_sz = menu_item_sz * 0.3f;
                vec2 p0 = mid + vec2(-cross_sz/2, -cross_sz/2);
                vec2 p1 = mid + vec2( cross_sz/2, -cross_sz/2);
                vec2 p2 = mid + vec2( cross_sz/2,  cross_sz/2);
                vec2 p3 = mid + vec2(-cross_sz/2,  cross_sz/2);
                glm::uvec4 color = {250, 50, 50, 255};
                line_absolute(p0, p2, color);
                line_absolute(p1, p3, color);
            } else {
                vec2 p0 = mid + vec2(-triangle_sz/2.f, -sqrtf(3)*triangle_sz/6.f);
                vec2 p1 = mid + vec2( triangle_sz/2.f, -sqrtf(3)*triangle_sz/6.f);
                vec2 p2 = mid + vec2(0.f, sqrtf(3.f)*triangle_sz/3.f);

                glm::uvec4 color = PlayerTriangle::triangleTypeMap[i].color;
                line_absolute(p0, p1, color);
                line_absolute(p1, p2, color);
                line_absolute(p2, p0, color);
            }
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
            if (menu_hover == 0) desc = "";
            drawer.text(desc, {10.f, 10.f}, 0.4f);

            // Draw name of hovered item to the left of the menu
            string name = PlayerTriangle::triangleTypeMap[menu_hover].name;
            if (menu_hover == 0) name = "Erase";
            auto box = get_menu_item_bounds(menu_hover);
            float name_x = box.first.x * (window_max.x - window_min.x) + window_min.x;
            float name_y = (box.second.y*drawer.aspect) * (window_max.y - window_min.y) + window_min.y;
            drawer.text_align_right(name, {name_x - 5.f, name_y - ((box.second.y - box.first.y)/2.f * (window_max.y - window_min.y) * drawer.aspect)}, 0.3f);
        }

        // Draw a white box over selected item
        if (menu_selected != -1) draw_selection_box(menu_selected, {255.f, 255.f, 255.f, 255.f});
    }

    { // Draw building area
        // Draw hovered triangle
        if (building_hovered) {
            vector<vec2> corners = player.cluster.getTriangleCorners(building_hover.first, building_hover.second);
            uvec4 color = {100.f, 100.f, 100.f, 255.f};
            if (menu_selected >= 1) {
                color = PlayerTriangle::triangleTypeMap[menu_selected].color;
                color.a = 128;
            }
            drawer.line(corners[0], corners[1], color);
            drawer.line(corners[1], corners[2], color);
            drawer.line(corners[2], corners[0], color);
        }
    }

    { // Warn player if structure is disconnected, otherwise draw "done" button
        if (disconnected) {
            drawer.text_align_centered("Structure is disconnected", {window_max.x/2, window_max.y*4.f/5.f}, 0.3f, {1.f, 0.4f, 0.4f});
        } else {
            vec3 color = done_hovered ? vec3(0.f, 1.f, 0.f) : vec3(.4f, .8f, .4f);
            uvec4 color4 = uvec4(int(color.r*255), int(color.g*255), int(color.b*255), 255);

            auto box = get_finished_button_bounds();
            vec2 p0 = {box.first.x, box.first.y};
            vec2 p1 = {box.second.x, box.first.y};
            vec2 p2 = {box.second.x, box.second.y};
            vec2 p3 = {box.first.x, box.second.y};
        
            line_absolute(p0, p1, color4);
            line_absolute(p1, p2, color4);
            line_absolute(p2, p3, color4);
            line_absolute(p3, p0, color4);

            vec2 mid = (box.first + box.second) / 2.f;
            float tx = mid.x * window_max.x;
            float ty = mid.y * drawer.aspect * window_max.y;
            drawer.text_align_centered("Done", {tx, ty}, 0.6f, color);
        }
    }
}

pair<vec2, vec2> Builder::get_menu_item_bounds(int idx) {
    static float sidelen = 0.05f;
    static float max_x = 1;
    static float max_y = max_x * (window_max.y - window_min.y)/(window_max.x - window_min.x);
    return make_pair(vec2(max_x - sidelen, max_y - idx*sidelen - sidelen),
                     vec2(max_x, max_y - idx*sidelen));
}

pair<int, int> Builder::get_triangle_coords(vec2 drawer_coords) {
    float bx = drawer_coords.x - 1;
    float by = drawer_coords.y - sqrtf(3.f) / 3.f;
    // Weird voodoo math calculations (https://www.boristhebrave.com/2021/05/23/triangle-grids/)
    int tx = (int)ceilf(bx - sqrtf(3.f) / 3.f * by);
    int ty = (int)floorf(sqrtf(3.f) * 2 / 3.f * by) + 1;
    int tz = (int)ceilf(-1.f * bx - sqrtf(3.f)/3.f * by);
    // Convert to own triangle coordinate system
    int cx = 2*tx + (tx+ty+tz)%2;
    int cy = ty;
    return make_pair(cx, cy);
}

pair<glm::vec2, glm::vec2> Builder::get_finished_button_bounds() {
    return make_pair(vec2(0.88, 0.02), vec2(0.98, 0.08));
}
