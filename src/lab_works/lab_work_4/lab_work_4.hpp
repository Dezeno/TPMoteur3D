#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		void _updateMatrices();

		TriangleMeshModel _triangleMeshModel;
		Camera			  _camera;
		Mat4f			  _modelMatrix = MAT4F_ID;
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		GLint  mvpLocation = -1;
		GLint  mvLocation = -1;
		GLint  normalLocation = -1;
		GLint  lightLocation  = -1;
		// ================

		// ================ Settings.
		Vec4f _bgColor			 = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;
		Vec3f _lightPosition	 = Vec3f( 500.f, 600.0f, -120.f );
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_4_HPP__
