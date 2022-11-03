#include "Drawer.hpp"
#include "DrawLines.hpp"
#include "Mesh.hpp"
#include <array>

Drawer::Drawer(glm::vec2 drawable_size) : lines(glm::mat4()) {
    this->center = {0.f, 0.f};
    this->scale = 1.f;

	// Not sure what these do but we can change when necessary
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	this->aspect = float(drawable_size.x) / float(drawable_size.y);
	glm::vec2 offset = {0.f, 0.f};
	glm::mat4 world_to_clip = glm::mat4(
		1.f / this->aspect, 0.0f, 0.0f, offset.x,
		0.0f, 1.f, 0.0f, offset.y,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

    this->lines = DrawLines(world_to_clip);
}

void Drawer::set_center(glm::vec2 c) {
    this->center = c;
}

void Drawer::set_width(float width) {
    this->scale = 1.f / (width/2) * this->aspect;
}

void Drawer::line(glm::vec2 p1, glm::vec2 p2, glm::uvec4 color) {
    glm::vec2 p1_ = (p1 - center) * scale;
    glm::vec2 p2_ = (p2 - center) * scale;
    this->lines.draw({p1_.x, p1_.y, 0.f},
                     {p2_.x, p2_.y, 0.f},
                     color
    );
}

void Drawer::circle(glm::vec2 p, float rad, glm::uvec4 color) {
    static float M_PI = acosf(-1.f);
	static std::array< glm::vec2, 64 > const circle = [](){
		std::array< glm::vec2, 64 > ret;
		for (uint32_t a = 0; a < ret.size(); ++a) {
			float ang = a / float(ret.size()) * 2.0f * float(M_PI);
			ret[a] = glm::vec2(std::cos(ang), std::sin(ang));
		}
		return ret;
	}();

    for (uint32_t a = 0; a < circle.size(); ++a) {
        this->line(
            p + rad * circle[a],
            p + rad * circle[(a + 1) % circle.size()],
            color);
    }
}
