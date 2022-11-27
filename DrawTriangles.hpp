#pragma once


#include <glm/glm.hpp>

#include <string>
#include <vector>

struct DrawTriangles {
	//Start drawing; will remember world_to_clip matrix:
	DrawTriangles(glm::mat4 const &world_to_clip);

	//draw a triangle a-b-c:
	void draw(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c, glm::u8vec4 const &color = glm::u8vec4(0xff));

	//Finish drawing (push attribs to GPU):
	~DrawTriangles();


	glm::mat4 world_to_clip;
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_) : Position(Position_), Color(Color_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
	};
	std::vector< Vertex > attribs;

};
