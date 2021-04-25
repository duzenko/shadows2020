#pragma once

struct GlSsbo {
	GLuint ssbo;
	GlSsbo();
	void Upload( std::vector<glm::vec4>& data );
};

