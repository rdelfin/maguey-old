#define GLM_FORCE_RADIANS

#include <maguey/debuggl.h>
#include <maguey/portable_gl.h>

const char* DebugGLErrorToString(int error) {
	switch (error) {
		case GL_NO_ERROR:
			return "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
			break;
		default:
			return "Unknown Error";
			break;
	}
	return "Unicorns Exist";
}
