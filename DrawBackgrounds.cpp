#include "DrawBackgrounds.hpp"
#include "BackgroundProgram.hpp"

#include "gl_errors.hpp"

#include <glm/gtc/type_ptr.hpp>

//All DrawLines instances share a vertex array object and vertex buffer, initialized at load time:

//n.b. declared static so they don't conflict with similarly named global variables elsewhere:
static GLuint vertex_buffer = 0;
static GLuint vertex_buffer_for_background_program = 0;

static Load< void > setup_buffers(LoadTagDefault, [](){
	//you may recognize this init code from DrawSprites.cpp:

	{ //set up vertex buffer:
		glGenBuffers(1, &vertex_buffer);
		//for now, buffer will be un-filled.
	}

	{ //vertex array mapping buffer for color_program:
		//ask OpenGL to fill vertex_buffer_for_color_program with the name of an unused vertex array object:
		glGenVertexArrays(1, &vertex_buffer_for_background_program);

		//set vertex_buffer_for_color_program as the current vertex array object:
		glBindVertexArray(vertex_buffer_for_background_program);

		//set vertex_buffer as the source of glVertexAttribPointer() commands:
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		//set up the vertex array object to describe arrays of PongMode::Vertex:
		glVertexAttribPointer(
			background_program->Position_vec4, //attribute
			3, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(DrawBackgrounds::Vertex), //stride
			(GLbyte *)0 + offsetof(DrawBackgrounds::Vertex, Position) //offset
		);
		glEnableVertexAttribArray(background_program->Position_vec4);
		//[Note that it is okay to bind a vec3 input to a vec4 attribute -- the w component will be filled with 1.0 automatically]

		glVertexAttribPointer(
			background_program->TexCoord_vec2, //attribute
			2, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(DrawBackgrounds::Vertex), //stride
			(GLbyte *)0 + offsetof(DrawBackgrounds::Vertex, TexCoord) //offset
		);
		glEnableVertexAttribArray(background_program->TexCoord_vec2);

		//done referring to vertex_buffer, so unbind it:
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//done setting up vertex array object, so unbind it:
		glBindVertexArray(0);
	}

	GL_ERRORS(); //PARANOIA: make sure nothing strange happened during setup
});


DrawBackgrounds::DrawBackgrounds(glm::mat4 const &world_to_clip_) : world_to_clip(world_to_clip_) {
}

void DrawBackgrounds::draw(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c, 
	glm::vec2 const &texCoord_a, glm::vec2 const &texCoord_b, glm::vec2 const &texCoord_c) {
	attribs.emplace_back(a, texCoord_a);
	attribs.emplace_back(b, texCoord_b);
	attribs.emplace_back(c, texCoord_c);
}

DrawBackgrounds::~DrawBackgrounds() {
	if (attribs.empty()) return;

	//based on DrawSprites.cpp :

	//upload vertices to vertex_buffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //set vertex_buffer as current
	glBufferData(GL_ARRAY_BUFFER, attribs.size() * sizeof(attribs[0]), attribs.data(), GL_STREAM_DRAW); //upload attribs array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set color_program as current program:
	glUseProgram(background_program->program);

	//upload OBJECT_TO_CLIP to the proper uniform location:
	glUniformMatrix4fv(background_program->OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(world_to_clip));

	//use the mapping vertex_buffer_for_color_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_background_program);

	//run the OpenGL pipeline:
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(attribs.size()));

	//reset vertex array to none:
	glBindVertexArray(0);

	//reset current program to none:
	glUseProgram(0);
}


