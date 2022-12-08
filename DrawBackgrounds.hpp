#pragma once


#include <glm/glm.hpp>

#include <string>
#include <vector>

struct DrawBackgrounds {
	//Start drawing; will remember world_to_clip matrix:
	DrawBackgrounds(glm::mat4 const &world_to_clip);

	//draw a triangle a-b-c:
	void draw(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c, 
		glm::vec2 const &texCoord_a, glm::vec2 const &texCoord_b, glm::vec2 const &texCoord_c);

	//Finish drawing (push attribs to GPU):
	~DrawBackgrounds();


	glm::mat4 world_to_clip;
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::vec2 const &TexCoord_) : Position(Position_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};
	std::vector< Vertex > attribs;

};
