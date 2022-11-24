// text rendering logic referenced from - https://learnopengl.com/In-Practice/Text-Rendering
#pragma once

// To draw text
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>
#include <hb-ft.h>

#include "GL.hpp"
#include <string>
#include "gl_errors.hpp"
#include "data_path.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <deque>
#include <map>


struct DrawText{
    DrawText();
    DrawText(char *fontfile);
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
	};

    std::map <char, Character> Characters;
	unsigned int VAO, VBO;
	GLuint program = 0;
	GLuint textColor = 0;
	glm::mat4 projection; 

    int WIDTH = 960;
    int HEIGHT = 540;

    // Helper functions 
    void load_char(char *fontfile);
    void draw_msg(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size, glm::vec3 color);
    void draw_msg_align_right(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size, glm::vec3 color);
};
