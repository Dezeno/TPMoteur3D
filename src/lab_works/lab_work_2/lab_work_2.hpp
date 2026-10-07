#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void disk( const Vec2f &c, int n, float r );
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		std::vector<Vec2f> _vertices;
		std::vector<unsigned int> _indexVertices;
		std::vector<Vec4f> _colorVertices;
		long double			   _time = 0;
		float				   _luminosite = 1.0f;
		// ================

		// ================ GL data.
		GLuint program = GL_INVALID_INDEX;
		GLuint vbo	   = GL_INVALID_INDEX;
		GLuint vbo2	   = GL_INVALID_INDEX;
		GLuint vao	   = GL_INVALID_INDEX;
		GLuint ebo	   = GL_INVALID_INDEX;
		GLint  location;
		GLint  brightnessLocation;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
