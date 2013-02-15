#ifndef GLUTILS_H
#define GLUTILS_H

#include <gl/glew.h>			// OpenGL Extension "autoloader"

class GLUtils
{
public:
    GLUtils();

    static int checkForOpenGLError(const char *, int);
    static void dumpGLInfo(bool dumpExtensions = false);

    /////////////////////////////////////////////////////////////////////////////////////
    // Load a .TGA file
    static GLbyte *ReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

    // Capture the frame buffer and write it as a .tga
    // Does not work on the iPhone
    #ifndef OPENGL_ES
    static GLint GrabScreenTGA(const char *szFileName);
    #endif
};

#endif // GLUTILS_H
