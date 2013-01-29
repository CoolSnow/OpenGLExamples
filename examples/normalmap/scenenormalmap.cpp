#include "scenenormalmap.h"
#include "soil.h"
#include "defines.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneNormalMap::SceneNormalMap()
{
}

void SceneNormalMap::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("bs_ears.obj");

    view = glm::lookAt(vec3(-1.0f,0.25f,2.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(100.0) );

    prog.setUniform("Light.Intensity", vec3(0.9f,0.9f,0.9f) );

    GLuint texIDs[2];

    // Copy brick texture to OpenGL
    glActiveTexture(GL_TEXTURE0);
    texIDs[0] = SOIL_load_OGL_texture("ogre_diffuse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y);
    glBindTexture(GL_TEXTURE_2D, texIDs[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.setUniform("ColorTex", 0);

    // Load moss texture file
    glActiveTexture(GL_TEXTURE1);
    texIDs[1] = SOIL_load_OGL_texture("ogre_normalmap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y);
    glBindTexture(GL_TEXTURE_2D, texIDs[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    prog.setUniform("NormalMapTex", 1);
}

void SceneNormalMap::update( float t )
{
    angle += 0.0001f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneNormalMap::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", view * vec4(10.0f * cos(angle),1.0f,10.0f * sin(angle),1.0f) );
    prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    ogre->render();
}

void SceneNormalMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneNormalMap::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    float c = 2.0f;
    projection = glm::ortho( -0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
}

void SceneNormalMap::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("normalmap.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("normalmap.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.link() )
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }

    prog.use();
}
