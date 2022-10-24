#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <random>

#define WIDTH 1280
#define HEIGHT 720


//compiles+links an OpenGL shader program from source.
// throws on compilation error.
GLuint gl_compile_program(std::string const &vertex_shader_source,std::string const &fragment_shader_source);

//PlayMode::init_shader(){
//	program = gl_compile_program(
//		//vertex shader:
//		"#version 330 core\n"
//		"layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n"
//		"out vec2 TexCoords;\n"
//		"uniform mat4 model;\n"
//		"uniform mat4 projection;\n"
//		"void main()\n"
//		"{\n"
//		"    TexCoords = vertex.zw;\n"
//		"    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
//		"}\n"
//	,
//		//fragment shader:
//		"#version 330 core\n"
//		"in vec2 TexCoords;\n"
//		"out vec4 color;\n"
//		"uniform sampler2D image;\n"
//		"uniform vec3 spriteColor;\n"
//		"void main()\n"
//		"{    \n"
//		"    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);\n"
//		"} \n"
//	);
//
//	vertices[] = { 
//        // pos      // tex
//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 0.0f, 0.0f, 
//    
//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 1.0f, 1.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f
//    };
//

//} 


PlayMode::PlayMode() {
	//init_shader();
	//std::string filename = "Player_Core.png";

	//load_png(filename, png_size, png_data, LowerLeftOrigin);
	//std::cout << png_size->x << std::endl; std::cout << png_size->y << std::endl;

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	//player movement:
	{
		//combine inputs into a move:
		constexpr float PlayerSpeed = 3.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	

	//update camera aspect ratio for drawable:
	//camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	//glUseProgram(lit_color_texture_program->program);
	//glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	//glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	//glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	//glUseProgram(0);
//
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.
//
	//scene.draw(*camera);



	//{ //use DrawLines to overlay some text:
	//	glDisable(GL_DEPTH_TEST);
	//	float aspect = float(drawable_size.x) / float(drawable_size.y);
	//	DrawLines lines(glm::mat4(
	//		1.0f / aspect, 0.0f, 0.0f, 0.0f,
	//		0.0f, 1.0f, 0.0f, 0.0f,
	//		0.0f, 0.0f, 1.0f, 0.0f,
	//		0.0f, 0.0f, 0.0f, 1.0f
	//	));
//
	//	constexpr float H = 0.09f;
	//	lines.draw_text("Mouse motion looks; WASD moves; escape ungrabs mouse",
	//		glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
	//		glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
	//		glm::u8vec4(0x00, 0x00, 0x00, 0x00));
	//	float ofs = 2.0f / drawable_size.y;
	//	lines.draw_text("Mouse motion looks; WASD moves; escape ungrabs mouse",
	//		glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
	//		glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
	//		glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	//}
	GL_ERRORS();
}
