
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glu.h>


// This is a class that loads TGA files into opengl textures
class TGA
{
private:
	// the handle for the texture in opengl
	GLuint textureHandle;
public:
	// Constructs and loads a TGA into opengl from the given image file path
	TGA(char* imagePath);

	// Returns the handle to the texture created from the image, for binding to opengl
	GLuint getTextureHandle(void);
};


