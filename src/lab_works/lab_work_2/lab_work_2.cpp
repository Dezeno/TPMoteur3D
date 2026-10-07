#include "imgui.h"
#include "lab_work_2.hpp"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <cmath>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteProgram( program );
		glDeleteBuffers( 1, &vbo );
		glDeleteBuffers( 1, &vbo2 );
		glDeleteBuffers( 1, &ebo );
		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;

		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );
		const GLchar *	  vSrc				= vertexShaderStr.c_str();
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();

		disk( Vec2f( 0.3f, -0.2f ), 64, 0.5f );

		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		glCreateBuffers( 1, &ebo );
		glCreateBuffers( 1, &vbo );
		glCreateBuffers( 1, &vbo2 );
		glNamedBufferData( vbo, _vertices.size() * sizeof( Vec2f ), _vertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( vbo2, _colorVertices.size() * sizeof( Vec4f ), _colorVertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( ebo, _indexVertices.size() * sizeof( int ), _indexVertices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );

		glEnableVertexArrayAttrib( vao, 1 );
		glVertexArrayAttribFormat( vao, 1, 4, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );
		glVertexArrayVertexBuffer( vao, 1, vbo2, 0, sizeof( Vec4f ) );
		glVertexArrayElementBuffer( vao, ebo );

		glVertexArrayAttribBinding( vao, 0, 0 );
		glVertexArrayAttribBinding( vao, 1, 1 );

		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glShaderSource( fragmentShader, 1, &fSrc, NULL );

		glCompileShader( vertexShader );
		glCompileShader( fragmentShader );

		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}

		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			return false;
		}

		program = glCreateProgram();

		glAttachShader( program, vertexShader );
		glAttachShader( program, fragmentShader );

		glLinkProgram( program );

		// Check if link is ok.
		GLint linked;
		glGetProgramiv( program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		location = glGetUniformLocation( program, "uTranslationX" );

		brightnessLocation = glGetUniformLocation( program, "uLuminosite" );
		glProgramUniform1f( program, brightnessLocation, _luminosite );

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		glUseProgram( program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::disk( const Vec2f &c, int n, float r )
	{
		_vertices.push_back( c );
		Vec3f color = getRandomVec3f();
		_colorVertices.push_back( Vec4f( color, 1.0f ) );

		for (int i=0; i<n; i++)
		{
			float theta = 2.0f * PIf * i / n;
			_vertices.push_back( Vec2f( c.x + r * std::cos(theta), c.y + r * std::sin(theta) ) );

			color = getRandomVec3f();
			_colorVertices.push_back( Vec4f( color, 1.0f ) );
		}

		for (int i = 0; i <= n; i++) 
		{
			_indexVertices.push_back( 0 );
			_indexVertices.push_back( i );
			_indexVertices.push_back( i % n + 1 );
		}
	}

	void LabWork2::animate( const float p_deltaTime ) 
	{ 
		glProgramUniform4f( program, location, glm::sin( _time )*0.5f, 0.0f, 0.0f, 0.0f );
		_time += p_deltaTime;
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( vao );
		glDrawElements( GL_TRIANGLES, _indexVertices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );
		ImGui::Text( "No setting available!" );
		if (ImGui::SliderFloat("Luminosité", &_luminosite, 0.0f, 1.0f)) 
		{
			glProgramUniform1f( program, brightnessLocation, _luminosite );
		}
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		ImGui::End();
	}

} // namespace M3D_ISICG
