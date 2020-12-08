#include "SpriteBatch.h"

#include <algorithm>

namespace GameEngine {

SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::init() {
    createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
    _sortType = sortType;
    _renderBatches.clear();
    // Have to delete any glyphs that remain so we don't have memory leaks!
    for (int i = 0; i < _glyphs.size(); i++) {
        delete _glyphs[i];
    }
    _glyphs.clear();
}

void SpriteBatch::end() {
    //sortGlyphs();
    createRenderBatches();
}
void SpriteBatch::drawLine(glm::vec2 _p1, glm::vec2 _p2, float _r, float _g, float _b, float _a, float _thickness) {
	Color color;
	color.r = _r;
	color.g = _g;
	color.b = _b;
	color.a = _a;
	if (_p1.x == _p2.x) {
		if (_p1.y < _p2.y) {
			draw(glm::vec4(_p1.x, _p1.y, _thickness, abs(_p1.y - _p2.y)), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, 1);
		}
		else {
			draw(glm::vec4(_p1.x, _p2.y, _thickness, abs(_p1.y - _p2.y)), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, 1);
		}
	}
	else if (_p1.y == _p2.y) {
		if (_p1.x < _p2.x) {
			draw(glm::vec4(_p1.x, _p1.y, abs(_p1.x - _p2.x), _thickness), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, 1);
		}
		else {
			draw(glm::vec4(_p2.x, _p1.y, abs(_p1.x - _p2.x), _thickness), glm::vec4(1, 1, 1, 1), NULL, 0.0f, color, 1);
		}
	}
	else {
		drawLine(_p1, _p2, color, _thickness);
	}
	
}
void SpriteBatch::drawLine(glm::vec2 _p1, glm::vec2 _p2, const Color& color, float _thickness) {

	Glyph* newGlyph = new Glyph;
	glm::vec4 uvRect = glm::vec4(1, 1, 1, 1);
	newGlyph->texture = NULL;
	newGlyph->depth = 1;

	_p1.x -= _thickness / 4;
	_p2.y -= _thickness / 4;
	_p2.x -= _thickness / 4;
	_p2.y -= _thickness / 4;
	//end 1
	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(_p1.x, _p1.y+_thickness);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(_p1.x, _p1.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);
	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(_p1.x+ _thickness, _p1.y);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(_p1.x+ _thickness, _p1.y + _thickness);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	newGlyph->topLeft.lightAlpha = 0;
	newGlyph->bottomLeft.lightAlpha = 0;
	newGlyph->bottomRight.lightAlpha = 0;
	newGlyph->topRight.lightAlpha = 0;
	_glyphs.push_back(newGlyph);

	//end 2
	Glyph* next = new Glyph;
	next->texture = NULL;
	next->depth = 1;
	next->topLeft.color = color;
	next->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
	next->bottomLeft.color = color;
	next->bottomLeft.setUV(uvRect.x, uvRect.y);
	next->bottomRight.color = color;
	next->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	next->topRight.color = color;
	next->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	next->topLeft.lightAlpha = 0;
	next->bottomLeft.lightAlpha = 0;
	next->bottomRight.lightAlpha = 0;
	next->topRight.lightAlpha = 0;
	next->topLeft.setPosition(_p2.x, _p2.y + _thickness);
	next->bottomLeft.setPosition(_p2.x, _p2.y);
	next->bottomRight.setPosition(_p2.x + _thickness, _p2.y);
	next->topRight.setPosition(_p2.x + _thickness, _p2.y + _thickness);
	_glyphs.push_back(next);

	Glyph* glyph3 = new Glyph;
	glyph3->texture = NULL;
	next->depth = 1;

	glyph3->topLeft.color = color;
	glyph3->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
	glyph3->bottomLeft.color = color;
	glyph3->bottomLeft.setUV(uvRect.x, uvRect.y);
	glyph3->bottomRight.color = color;
	glyph3->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	glyph3->topRight.color = color;
	glyph3->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	glyph3->topLeft.lightAlpha = 0;
	glyph3->bottomLeft.lightAlpha = 0;
	glyph3->bottomRight.lightAlpha = 0;
	glyph3->topRight.lightAlpha = 0;
	glyph3->topLeft.setPosition(_p1.x, _p1.y);
	glyph3->bottomLeft.setPosition(_p1.x+ _thickness, _p1.y+ _thickness);
	glyph3->bottomRight.setPosition(_p2.x, _p2.y);
	glyph3->topRight.setPosition(_p2.x + _thickness, _p2.y+ _thickness);
	_glyphs.push_back(glyph3)
		;
	Glyph* glyph4 = new Glyph;
	glyph3->texture = NULL;
	next->depth = 1;

	glyph4->topLeft.color = color;
	glyph4->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
	glyph4->bottomLeft.color = color;
	glyph4->bottomLeft.setUV(uvRect.x, uvRect.y);
	glyph4->bottomRight.color = color;
	glyph4->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	glyph4->topRight.color = color;
	glyph4->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	glyph4->topLeft.lightAlpha = 0;
	glyph4->bottomLeft.lightAlpha = 0;
	glyph4->bottomRight.lightAlpha = 0;
	glyph4->topRight.lightAlpha = 0;
	glyph4->topLeft.setPosition(_p1.x+ _thickness, _p1.y);
	glyph4->bottomLeft.setPosition(_p1.x, _p1.y+ _thickness);
	glyph4->bottomRight.setPosition(_p2.x+ _thickness, _p2.y);
	glyph4->topRight.setPosition(_p2.x, _p2.y+ _thickness);
	_glyphs.push_back(glyph4);
	/*
	newGlyph->topLeft.setPosition(_p1.x, _p1.y + 1);
	newGlyph->bottomLeft.setPosition(_p1.x, _p1.y);
	newGlyph->bottomRight.setPosition(_p2.x + 1, _p2.y);
	newGlyph->topRight.setPosition(_p2.x + 1, _p2.y + 1);
	_glyphs.push_back(newGlyph);
	newGlyph->topLeft.setPosition(_p1.x, _p1.y + 1);
	newGlyph->bottomLeft.setPosition(_p1.x, _p1.y);
	newGlyph->bottomRight.setPosition(_p1.x + 1, _p1.y);
	newGlyph->topRight.setPosition(_p1.x + 1, _p1.y + 1);

	_glyphs.push_back(newGlyph);*/
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {

    Glyph* newGlyph = new Glyph;

    newGlyph->texture = texture;
    newGlyph->depth = depth;

    newGlyph->topLeft.color = color;
    newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    newGlyph->bottomLeft.color = color;
    newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
    newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

    newGlyph->bottomRight.color = color;
    newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    newGlyph->topRight.color = color;
    newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	if (texture != NULL) {
		Color colorForTextures;
		colorForTextures.r = 0;
		colorForTextures.g = 0;
		colorForTextures.b = 0;
		colorForTextures.a = 0;
		newGlyph->topLeft.color = colorForTextures;
		newGlyph->bottomLeft.color = colorForTextures;
		newGlyph->bottomRight.color = colorForTextures;
		newGlyph->topRight.color = colorForTextures;
	}
	newGlyph->topLeft.lightAlpha = 1;
	newGlyph->bottomLeft.lightAlpha = 1;
	newGlyph->bottomRight.lightAlpha = 1;
	newGlyph->topRight.lightAlpha = 1;
    _glyphs.push_back(newGlyph);
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth) {

	Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0;
	draw(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, float _lightAlpha, float angle) {

	Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0;
	draw(destRect, uvRect, texture, depth, color, _lightAlpha, angle, false);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, float _lightAlpha, float angle, bool _flip)
{
	Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0;
	draw(destRect, uvRect, texture, depth, color, _lightAlpha, angle, _flip);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float _lightAlpha, float angle, bool flip) {


	Glyph* newGlyph = new Glyph;

	float m = destRect.z*sin(angle);
	float n = destRect.z*cos(angle);
	float z = destRect.w*sin(angle);
	float j = destRect.w*cos(angle);

	newGlyph->texture = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRect.x + z, destRect.y + j);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRect.x + n, destRect.y - m);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRect.x + z + n, destRect.y + j - m);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	if (texture != NULL) {
		Color colorForTextures;
		colorForTextures.r = 0;
		colorForTextures.g = 0;
		colorForTextures.b = 0;
		colorForTextures.a = 0;
		newGlyph->topLeft.color = colorForTextures;
		newGlyph->bottomLeft.color = colorForTextures;
		newGlyph->bottomRight.color = colorForTextures;
		newGlyph->topRight.color = colorForTextures;
	}
	newGlyph->topLeft.lightAlpha = _lightAlpha;
	newGlyph->bottomLeft.lightAlpha = _lightAlpha;
	newGlyph->bottomRight.lightAlpha = _lightAlpha;
	newGlyph->topRight.lightAlpha = _lightAlpha;
	if (flip)
	{
		newGlyph->bottomRight.position.x = newGlyph->bottomLeft.position.x - (newGlyph->bottomRight.position.x - newGlyph->bottomLeft.position.x);
		newGlyph->topRight.position.x = newGlyph->bottomLeft.position.x - (newGlyph->topRight.position.x - newGlyph->bottomLeft.position.x);
		newGlyph->topLeft.position.x = newGlyph->bottomLeft.position.x - (newGlyph->topLeft.position.x - newGlyph->bottomLeft.position.x);
	}
	_glyphs.push_back(newGlyph);
}
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float _lightAlpha) {

	Glyph* newGlyph = new Glyph;

	newGlyph->texture = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	if (texture != NULL) {
		Color colorForTextures;
		colorForTextures.r = 0;
		colorForTextures.g = 0;
		colorForTextures.b = 0;
		colorForTextures.a = 0;
		newGlyph->topLeft.color = colorForTextures;
		newGlyph->bottomLeft.color = colorForTextures;
		newGlyph->bottomRight.color = colorForTextures;
		newGlyph->topRight.color = colorForTextures;
	}
	newGlyph->topLeft.lightAlpha = _lightAlpha;
	newGlyph->bottomLeft.lightAlpha = _lightAlpha;
	newGlyph->bottomRight.lightAlpha = _lightAlpha;
	newGlyph->topRight.lightAlpha = _lightAlpha;
	_glyphs.push_back(newGlyph);
}
void SpriteBatch::renderBatch() {

    // Bind our VAO. This sets up the opengl state we need, including the 
    // vertex attribute pointers and it binds the VBO
    glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++) {
		if (_renderBatches[i].texture != NULL) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
        glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
    }

    glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {
    // This will store all the vertices that we need to upload
    std::vector <Vertex> vertices;
    // Resize the buffer to the exact size we need so we can treat
    // it like an array
    vertices.resize(_glyphs.size() * 6);

    if (_glyphs.empty()) {
        return;
    }

    int offset = 0; // current offset
    int cv = 0; // current vertex

    //Add the first batch
    _renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
    vertices[cv++] = _glyphs[0]->topLeft;
    vertices[cv++] = _glyphs[0]->bottomLeft;
    vertices[cv++] = _glyphs[0]->bottomRight;
    vertices[cv++] = _glyphs[0]->bottomRight;
    vertices[cv++] = _glyphs[0]->topRight;
    vertices[cv++] = _glyphs[0]->topLeft;
    offset += 6;

    //Add all the rest of the glyphs
    for (int cg = 1; cg < _glyphs.size(); cg++) {

        // Check if this glyph can be part of the current batch
        if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
            // Make a new batch
            _renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
        } else {
            // If its part of the current batch, just increase numVertices
            _renderBatches.back().numVertices += 6;
        }
        vertices[cv++] = _glyphs[cg]->topLeft;
        vertices[cv++] = _glyphs[cg]->bottomLeft;
        vertices[cv++] = _glyphs[cg]->bottomRight;
        vertices[cv++] = _glyphs[cg]->bottomRight;
        vertices[cv++] = _glyphs[cg]->topRight;
        vertices[cv++] = _glyphs[cg]->topLeft;
        offset += 6;
    }

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // Orphan the buffer (for speed)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertexArray() {

	// Generate the VAO if it isn't already generated
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);


		// Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(_vao);

		//G enerate the VBO if it isn't already generated
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Tell opengl what attribute arrays we need
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, lightAlpha));
		

		glBindVertexArray(0);

	}
}
	void SpriteBatch::sortGlyphs() {

		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}