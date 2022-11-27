#pragma once

#include <glm/glm.hpp>
#include "DrawLines.hpp"
#include "DrawTriangles.hpp"

struct Drawer {
    glm::vec2 center = {0.f, 0.f};
    float scale = 1.f;
    float aspect = 1.f;
    DrawLines lines;
    DrawTriangles triangles;

    Drawer(glm::vec2 drawable_size);

    void set_center(glm::vec2 c);
    void set_width(float width);
    void line(glm::vec2 p1, glm::vec2 p2, glm::uvec4 color);
    void circle(glm::vec2 p, float rad, glm::uvec4 color);
    void triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::uvec4 color);
    void text(std::string text, glm::vec2 at, float scale);
};
