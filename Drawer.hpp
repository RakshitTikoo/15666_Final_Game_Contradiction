#pragma once

#include <glm/glm.hpp>
#include "DrawLines.hpp"
#include "DrawText.hpp"
#include "DrawTriangles.hpp"
#include "TriangleCluster.hpp"

using namespace std;

struct Drawer {
    glm::vec2 drawable_size;
    glm::vec2 center = {0.f, 0.f};
    float width = 1.f;
    float aspect = 1.f;
    DrawLines lines;
    DrawText TextRenderer;
    DrawTriangles triangles;

    Drawer(glm::vec2 drawable_size, DrawText& textRenderer);

    void set_center(glm::vec2 c);
    void set_width(float width);
    void line(glm::vec2 p1, glm::vec2 p2, glm::uvec4 color);
    void circle(glm::vec2 p, float rad, glm::uvec4 color);
    void solidCircle(glm::vec2 p, float rad, glm::uvec4 color);
    void text(std::string text, glm::vec2 at, float scale, glm::vec3 color = {1.f, 1.f, 1.f});
    void text_align_right(std::string text, glm::vec2 at, float scale, glm::vec3 color = {1.f, 1.f, 1.f});
    void text_align_centered(std::string text, glm::vec2 at, float scale, glm::vec3 color = {1.f, 1.f, 1.f});
    void triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::uvec4 color);
};