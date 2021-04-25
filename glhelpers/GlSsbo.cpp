#include "pch.h"
#include "GlSsbo.h"

GlSsbo::GlSsbo() {
	glGenBuffers( 1, &ssbo );
}

void GlSsbo::Upload( std::vector<glm::vec4>& data ) {
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, ssbo );
	glBufferData( GL_SHADER_STORAGE_BUFFER, data.size() * sizeof( data[0] ), &data[0], GL_DYNAMIC_COPY );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, ssbo );
}
