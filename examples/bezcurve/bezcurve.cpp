#include <stdio.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "scenebezcurve.h"

Scene * scene = NULL;

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    scene->resize(w, h);
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    scene = new SceneBezCurve();
    scene->initScene();
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    scene->render();
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
    int running = GL_TRUE;
    // Initialize GLFW
    if( !glfwInit() )
        exit( EXIT_FAILURE );

    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

    // Open an OpenGL window
    if( !glfwOpenWindow( 800,600, 0,0,0,0,0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    GLenum err = glewInit();
    if ( GLEW_OK != err) {
        fprintf(stderr , "GLEW Error: %s\n" , glewGetErrorString (err));
        exit( EXIT_FAILURE );
    }

    SetupRC();
    glfwSetWindowSizeCallback(ChangeSize);

    // Main loop
    while( running )
    {
        //// OpenGL rendering goes here...
        RenderScene();
        glfwSwapBuffers();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
            glfwGetWindowParam( GLFW_OPENED );
    }
    // Close window and terminate GLFW
    glfwTerminate();
    // Exit program
    exit( EXIT_SUCCESS );
}
