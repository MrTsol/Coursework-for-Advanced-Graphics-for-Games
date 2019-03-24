#pragma once
#include "../../NCLGL/mesh.h"
#include <vector>

struct Font {
	GLuint	texture;
	int		xCount;
	int		yCount;

	Font(GLuint tex, unsigned int xCount, unsigned int yCount) {
		this->texture = tex;
		this->xCount = xCount;
		this->yCount = yCount;
	}
	~Font() {
		glDeleteTextures(1, &texture);
	}
};

class Text : public Mesh {
public:
	Text(const std::string &text, const Font &font);
	~Text(void);
protected:

	const Font& font;
};