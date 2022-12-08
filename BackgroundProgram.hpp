#pragma once

#include "GL.hpp"
#include "Load.hpp"

//Shader program that draws transformed, colored vertices:
struct BackgroundProgram {
	BackgroundProgram();
	~BackgroundProgram();

	GLuint program = 0;
	//Attribute (per-vertex variable) locations:
	GLuint Position_vec4 = -1U;
	GLuint TexCoord_vec2 = -1U;
	//Uniform (per-invocation variable) locations:
	GLuint OBJECT_TO_CLIP_mat4 = -1U;
	//Textures:
	// none
};

extern Load< BackgroundProgram > background_program;
