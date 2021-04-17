#include "pch.h"
#include "GlSsbo.h"

GlSsbo::GlSsbo( int ssboSize, void* ssboData ) {
	GLuint ssbo;
	glGenBuffers( 1, &ssbo );
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, ssbo );
	glBufferData( GL_SHADER_STORAGE_BUFFER, ssboSize, ssboData, GL_DYNAMIC_COPY );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, ssbo );
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
}