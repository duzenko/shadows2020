#include "pch.h"
#include "GlProgram.h"

std::string readGlsl( std::string name ) {
    std::ifstream in( "glsl\\" + name );
    std::string contents( ( std::istreambuf_iterator<char>( in ) ),
        std::istreambuf_iterator<char>() );
    return contents;
}

std::map<GLenum, const char*> knownTypes = { {GL_VERTEX_SHADER, ".vs"}, {GL_FRAGMENT_SHADER, ".fs"} };

void loadShader( std::string& name, GLenum shaderType, GLuint program ) {
    auto fileExt = knownTypes[shaderType];
    std::string main = readGlsl( name + fileExt );
    std::string random = readGlsl( "random.glsl" );
    std::string common = readGlsl( "common.glsl" );
    const GLchar* ptr[] = { common.c_str(), random.c_str(), main.c_str() };
    auto shader = glCreateShader( shaderType );
    glShaderSource( shader, 3, ptr, NULL );

    glCompileShader( shader );

    GLint isCompiled = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
    if ( isCompiled == GL_FALSE ) {
        GLint maxLength = 0;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( maxLength );
        glGetShaderInfoLog( shader, maxLength, &maxLength, &errorLog[0] );
        std::cout << name + fileExt << " compile failed\n";
        for ( auto i : errorLog )
            std::cout << i;

        glDeleteShader( shader );
    }

    glAttachShader( program, shader );
}

GLuint loadGlProgram( std::string &name ) {
    auto program = glCreateProgram();
    loadShader( name, GL_VERTEX_SHADER, program );
    loadShader( name, GL_FRAGMENT_SHADER, program );
    glLinkProgram( program );
    GLint link_ok = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &link_ok );
    if ( !link_ok ) {
        GLint maxLength = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( maxLength + 1 );
        glGetProgramInfoLog( program, maxLength, &maxLength, &errorLog[0] );
        std::cout << name << " link failed\n";
        for ( auto i : errorLog )
            std::cout << i;
    }
    glUseProgram( program );
    return program;
}

GlProgram::GlProgram( const char* name ) : limit( 1 ), lightPos( 2 ), matProjection( 3 ), matView(4), soften(5), time(6), lightSize(7), vertexCount(8) {
    std::string s( name );
    handle = loadGlProgram( s );
}

void GlProgram::Use() {
    glUseProgram( handle );
}

void GlProgramUniform::operator = ( int value ) {
    glUniform1i( location, value );
}

void GlProgramUniform::operator = ( float value ) {
    glUniform1f( location, value );
}

void GlProgramUniform::operator = ( glm::vec2& value ) {
    glUniform2fv( location, 1, glm::value_ptr( value ) );
}

void GlProgramUniform::operator = ( glm::mat4& value ) {
    glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( value ) );
}