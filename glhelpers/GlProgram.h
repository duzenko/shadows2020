#pragma once

struct GlProgramUniform {
	int location;
	GlProgramUniform( int location ) : location( location ) {}
	void operator = ( int value );
	void operator = ( float value );
	void operator = ( glm::vec2& value );
	void operator = ( glm::mat4& value );
};

struct GlProgram {
	GlProgramUniform limit;
	GlProgramUniform lightPos;
	GlProgramUniform matProjection;
	GlProgramUniform matView;
	GlProgramUniform soften;
	GlProgramUniform time;
	GlProgramUniform lightSize;
	GlProgram();
};