#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_3.hpp"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3()
	{
		glDeleteProgram( program );
		glDeleteBuffers( 1, &_cube.vbo );
		glDeleteBuffers( 1, &_cube.vbo2 );
		glDeleteBuffers( 1, &_cube.ebo );
		glDisableVertexArrayAttrib( _cube.vao, 0 );
		glDeleteVertexArrays( 1, &_cube.vao );
	}

	bool LabWork3::init()
	{
		std::cout << "Initializing lab work 3..." << std::endl;

		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw3.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw3.frag" );
		const GLchar *	  vSrc				= vertexShaderStr.c_str();
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();

		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

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

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		_createCube();
		_initBuffers();

		glEnable( GL_DEPTH_TEST );
		
		glUseProgram( program );

		location = glGetUniformLocation( program, "uMVPMatrix" );

		_camera.setPosition( Vec3f( 0.0f, 1.0f, 3.0f ) );
		_camera.setScreenSize(1280, 720);
		_camera.setFovy( _fovy );
		_camera.setOrthoSize( _orthoSize );
		_camera.setPerspective( _perspective );
		_updateMVPMatrix();

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::_createCube() 
	{
		_cube.posVertices = {
			{ -0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, -0.5f, 0.5f },
			{ -0.5f, -0.5f, 0.5f },
			{ -0.5f, 0.5f, -0.5f },
			{ 0.5f, 0.5f, -0.5f }, 
			{ 0.5f, -0.5f, -0.5f }, 
			{ -0.5f, -0.5f, -0.5f }
		};
		_cube.colorVertices = { getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f(),
								getRandomVec3f(), getRandomVec3f(), getRandomVec3f(), getRandomVec3f() };
		_cube.indexVertices = { 
			0, 1, 2, 0, 3, 2, // avant
			4, 5, 6, 4, 7, 6, // arrière
			4, 0, 3, 4, 3, 7, // gauche
			1, 5, 6, 1, 6, 2, // droite
			4, 5, 1, 4, 1, 0, // haut
			3, 2, 6, 3, 6, 7  // bas
		};
		_cube.transformation = glm::scale( _cube.transformation, glm::vec3( 0.8f ) );
	}

	void LabWork3::_initBuffers() 
	{
		glCreateBuffers( 1, &_cube.vbo );
		glCreateBuffers( 1, &_cube.vbo2 );
		glCreateBuffers( 1, &_cube.ebo );
		glNamedBufferData(_cube.vbo, _cube.posVertices.size() * sizeof( Vec3f ), _cube.posVertices.data(), GL_STATIC_DRAW );
		glNamedBufferData(_cube.vbo2, _cube.colorVertices.size() * sizeof( Vec3f ), _cube.colorVertices.data(), GL_STATIC_DRAW );
		glNamedBufferData(_cube.ebo, _cube.indexVertices.size() * sizeof( unsigned int ), _cube.indexVertices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_cube.vao );
		glEnableVertexArrayAttrib( _cube.vao, 0 );
		glVertexArrayAttribFormat( _cube.vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );

		glEnableVertexArrayAttrib( _cube.vao, 1 );
		glVertexArrayAttribFormat( _cube.vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( _cube.vao, 0, _cube.vbo, 0, sizeof( Vec3f ) );
		glVertexArrayVertexBuffer( _cube.vao, 1, _cube.vbo2, 0, sizeof( Vec3f ) );
		glVertexArrayElementBuffer( _cube.vao, _cube.ebo );

		glVertexArrayAttribBinding( _cube.vao, 0, 0 );
		glVertexArrayAttribBinding( _cube.vao, 1, 1 );
	}

	void LabWork3::_updateMVPMatrix()
	{
		const Mat4f mvp = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _cube.transformation;
		glProgramUniformMatrix4fv( program, location, 1, GL_FALSE, glm::value_ptr( mvp ) );
	}

	void LabWork3::animate( const float p_deltaTime ) 
	{ 
		_cube.transformation = glm::rotate( _cube.transformation, p_deltaTime, Vec3f( 0.f, 1.f, 1.f ) );
		_updateMVPMatrix();
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray( _cube.vao );
		glDrawElements( GL_TRIANGLES, _cube.indexVertices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateMVPMatrix();
		}
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );
		ImGui::Text( "No setting available!" );

		if ( ImGui::RadioButton( "Perspective", _perspective ) )
		{
			_perspective = true;
			_camera.setPerspective( true );
			_updateMVPMatrix();
		}
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Orthographique", !_perspective ) )
		{
			_perspective = false;
			_camera.setPerspective( false );
			_updateMVPMatrix();
		}

		if ( _perspective )
		{
			if ( ImGui::SliderFloat( "Fovy", &_fovy, 10.f, 120.f, "%.1f deg" ) )
			{
				_camera.setFovy( _fovy );
				_updateMVPMatrix();
			}
		}
		else
		{
			if ( ImGui::SliderFloat( "Taille ortho", &_orthoSize, 0.5f, 10.f, "%.2f" ) )
			{
				_camera.setOrthoSize( _orthoSize );
				_updateMVPMatrix();
			}
		}

		ImGui::End();
	}

} // namespace M3D_ISICG
