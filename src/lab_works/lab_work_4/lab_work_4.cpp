#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_4.hpp"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4()
	{
		glDeleteProgram( program );
		_triangleMeshModel.cleanGL();
	}

	bool LabWork4::init()
	{
		std::cout << "Initializing lab work 4..." << std::endl;

		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		const std::string vertexShaderStr	= readFile( _shaderFolder + "mesh.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );
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

		_triangleMeshModel.load( "conference", "data/models/conference/conference.obj" );

		_modelMatrix = glm::scale( _modelMatrix, glm::vec3( 0.003, 0.003, 0.003 ) );

		glEnable( GL_DEPTH_TEST );

		glUseProgram( program );

		mvpLocation = glGetUniformLocation( program, "uMVPMatrix" );
		mvLocation = glGetUniformLocation( program, "uMVMatrix" );
		normalLocation = glGetUniformLocation( program, "uNormalMatrix" );
		lightLocation = glGetUniformLocation( program, "uLightPosition" );

		_camera.setScreenSize( 1280, 720 );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork4::_updateMatrices()
	{
		const Mat4f mv = _camera.getViewMatrix() * _modelMatrix;
		const Mat4f mvp = _camera.getProjectionMatrix() * mv;
		const Mat4f normalMatrix = glm::transpose( glm::inverse( mv ) );
		const Vec3f light = _camera.getViewMatrix() * _modelMatrix * Vec4f( _lightPosition, 1.0f );
		
		glProgramUniformMatrix4fv( program, mvLocation, 1, GL_FALSE, glm::value_ptr( mv ) );
		glProgramUniformMatrix4fv( program, mvpLocation, 1, GL_FALSE, glm::value_ptr( mvp ) );
		glProgramUniformMatrix4fv( program, normalLocation, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
		glProgramUniform3fv( program, lightLocation, 1, glm::value_ptr( light ) );
	}

	void LabWork4::animate( const float p_deltaTime ){}

	void LabWork4::render()
	{ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		_updateMatrices();

		_triangleMeshModel.render( program );
	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateMatrices();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateMatrices();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateMatrices();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateMatrices();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateMatrices();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateMatrices();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateMatrices();
		}
	}

	void LabWork4::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
