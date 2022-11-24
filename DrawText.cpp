
#include "DrawText.hpp"


//compiles+links an OpenGL shader program from source.
GLuint gl_compile_program(
	std::string const &vertex_shader_source,
	std::string const &fragment_shader_source);

DrawText::DrawText(){
    
}

DrawText::DrawText(char *fontfile){
    load_char(fontfile);
}

void DrawText::load_char(char *fontfile) {
    program = gl_compile_program(
		//vertex shader:
		"#version 330 core\n"
		"layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
    	"gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    	"TexCoords = vertex.zw;\n"
		"}\n"
	,
		//fragment shader:
		"#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D text;\n"
		"uniform vec3 textColor;\n"
		"void main()\n"
		"{\n"
		"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"color = vec4(textColor, 1.0) * sampled;\n"
		"}\n"
	);

	/* Initialize FreeType and create FreeType font face. */
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // compile and setup the shader
    // ----------------------------
    glUseProgram(program);
    projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

		
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontfile, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawText::draw_msg(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size, glm::vec3 color)
{
	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
	float x_ori = x;
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
		if(*c == '\n') { // new line logic
			y = y - 50.0f*scale;
			x = x_ori;
			continue;
		}
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
		//printf("xpos %f ypos %f\n", xpos, ypos);
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void DrawText::draw_msg_align_right(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size, glm::vec3 color) {
    float x_ori = x;
    float width = 0;
    x = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
		if(*c == '\n') { // new line logic
			x = x_ori;
			continue;
		}
        Character ch = Characters[*c];
        x += (ch.Advance >> 6) * scale;
        width = fmax(width, x);
    }
    draw_msg(text, x_ori - width, y, scale, drawable_size, color);
}

void DrawText::draw_msg_align_centered(std::string text, float x, float y, float scale, glm::uvec2 const &drawable_size, glm::vec3 color) {
    float x_ori = x;
    float y_ori = y;
    float width = 0;
    float lines = 0;
    float height = 0;
    x = 0;
    y = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
		if(*c == '\n') { // new line logic
			x = x_ori;
            lines += 50.f;
            height = 0.f;
			continue;
		}
        Character ch = Characters[*c];
        float h = ch.Size.y * scale;
        height = fmax(height, h);
        x += (ch.Advance >> 6) * scale;
        width = fmax(width, x);
    }
    height += lines;
    draw_msg(text, x_ori - width/2.f, y_ori - height/2.f, scale, drawable_size, color);
}
