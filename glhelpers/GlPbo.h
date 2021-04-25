#pragma once

#include <glad/glad.h>

struct GlPbo {
	GLuint handle=0;
	int size=0;

	GlPbo() {
		glGenBuffers( 1, &handle );
		glBindBuffer( GL_PIXEL_PACK_BUFFER, handle );
		glBindBuffer( GL_PIXEL_UNPACK_BUFFER, handle );
	}

	void Resize(int newSize) {
		if ( size == newSize )
			return;
		size = newSize;
		glBufferData( GL_PIXEL_PACK_BUFFER, newSize, nullptr, GL_DYNAMIC_COPY );
	}
};

