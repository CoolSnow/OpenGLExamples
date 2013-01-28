                            // Triangle.cpp
// Our first OpenGL program that will just draw a triangle on the screen.

#include <stdio.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <glut/glut.h>          // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>            // Windows FreeGlut equivalent
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
    
#include "vboplane.h"
#include "vboteapot.h"
#include "glslprogram.h"

GLSLProgram prog;
VBOPlane *   plane = NULL;
VBOTeapot * teapot = NULL;

mat4 model;
mat4 view;
mat4 projection;

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    glViewport(0,0,w,h);
    projection = glm::perspective(70.0f, (float)w/h, 0.3f, 100.0f);
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    prog.compileShaderFromFile("subroutine.vert", GLSLShader::VERTEX);
    prog.compileShaderFromFile("subroutine.frag", GLSLShader::FRAGMENT);
    prog.link();
    prog.use();

    glEnable(GL_DEPTH_TEST);

    plane  = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, mat4(1.0f));

    view = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f,1.0f));
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Shininess", 100.0f);
}

void setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint programHandle = prog.getHandle();
    GLuint adsIndex = glGetSubroutineIndex( programHandle, GL_VERTEX_SHADER, "phongModel" );
    GLuint diffuseIndex = glGetSubroutineIndex(programHandle, GL_VERTEX_SHADER, "diffuseOnly");

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &adsIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(-3.0f,-1.5f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &diffuseIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(3.0f,-1.5f, 0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("phong");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    SetupRC();

    glutMainLoop();
    return 0;
}
