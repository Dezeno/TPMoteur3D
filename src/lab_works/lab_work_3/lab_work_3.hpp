#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		struct Mesh
		{
			std::vector<Vec3f>		  posVertices;
			std::vector<Vec3f>		  colorVertices;
			std::vector<unsigned int> indexVertices;
			Mat4f					  transformation = glm::mat4(1.0f);
			GLuint					  vbo = GL_INVALID_INDEX;
			GLuint					  vbo2 = GL_INVALID_INDEX;
			GLuint					  vao = GL_INVALID_INDEX;
			GLuint					  ebo = GL_INVALID_INDEX;
		};

		void _createCube();
		void _initBuffers();
		void _updateMVPMatrix();

		Mesh _cube;
		Camera _camera;
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		GLint  location;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float _fovy				 = 60.f;
		bool  _perspective		 = true;
		float _orthoSize		 = 2.f;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
