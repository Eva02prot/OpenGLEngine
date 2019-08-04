//
//  scene1.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "engine.h"
#include "camera.hpp"
#include "light.hpp"
#include "uimgr.hpp"
#include "label.hpp"
#include "button.hpp"
#include "uievent.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "shadow.hpp"

#define TY_Scene1 0
#define TY_Scene2 1
#define TY_Scene3 2
#define TY_Scene4 3
#define TY_Scene5 4
#define TY_Scene6 5
#define TY_Scene7 6

using namespace engine;

class Scene
{
    
    #define NUM_FRUSTUM_CORNERS 8
    
public:
    
    virtual ~Scene()
    {
        SAFE_DELETE(camera);
        SAFE_DELETE(skybox);
        SAFE_DELETE(light);
        SAFE_DELETE(shadow);
        SAFE_DELETE(debugShader);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glCheckError();
    }
    
    virtual bool drawShadow() { return true; }
  
    virtual glm::vec3 getCameraPos() { return glm::vec3(0.0f,0.0f,3.0f); }
    
    virtual std::string getSkybox() { return "lake"; }
    
    virtual bool isEquirectangularMap() { return false; }
    
    virtual uint bufferBit() { return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ; }
    
    /*
     init: camera-> skybox-> light-> scene-> ui
     */
    void Initial()
    {
        camera = new Camera(getCameraPos());
        skybox = new Skybox(camera, getSkybox(), isEquirectangularMap());
        loop(CASCACDE_NUM) lightMatrix[i] = mat4(1);
        InitLight();
        InitScene();
        if(drawShadow())
        {
            debugShader = new Shader("debug.vs", "debug.fs");
            debugShader->attach("_DEBUG_DEPTH_");
            InitQuad(&quadVAO, &quadVBO, debugShader);
            InitShadow();
        }
        DrawUI();
    }
    
    virtual int getType() = 0;
    
    virtual void InitLight() = 0;
    
    virtual void InitScene() { }
    
    virtual void InitShadow() { }
    
    virtual void DrawUI() { }
    
    virtual void DrawShadow() { }
    
    virtual void DrawScene() { }
    
    virtual void OnKeyboard(GLFWwindow *window) { }
    
    void ClearScene()
    {
        uint bit = bufferBit();
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        if(bit & GL_STENCIL_BUFFER_BIT)
        {
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilMask(0x00);
        }
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(bit);
    }
    
    void ClearDepth()
    {
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    
    void DrawScenes()
    {
        timeValue = GetRuntime();
        if(shadow && drawShadow())
        {
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_WIDTH);
            DrawShadow();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        ClearScene();
        glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGTH);
        DrawScene();
        if(drawShadow() && debug) RenderShadow();
        if(skybox) skybox->Draw();
    }
    
    void RebuildSky()
    {
        delete skybox;
        skybox = new Skybox(camera, getSkybox(), isEquirectangularMap());
    }
    
    void ProcessKeyboard(GLFWwindow *window, float deltatime)
    {
        OnKeyboard(window);
        if(camera)
        {
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->ProcessKeyboard(LEFT, deltatime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->ProcessKeyboard(RIGHT, deltatime);
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera->ProcessKeyboard(FORWARD, deltatime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->ProcessKeyboard(BACKWARD, deltatime);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                float timeValue = GetRuntime() * 0.04f;
                float ang = radians(timeValue);
                vec3 center = vec3(0.0f, 0.0f, -2.0f);
                vec3 pos = camera->Position;
                vec3 npos = pos;
                npos.x = (pos.x - center.x) * cos(ang) - (pos.z- center.z)*sin(ang) + center.x;
                npos.z = (pos.z - center.z) * cos(ang) + (pos.x - center.x) * sin(ang) + center.z;
                camera->RotateAt(npos, center);
            }
            if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) skybox->Equirect2Cube();
        }
        if(light)
        {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                light->UpdateX(-deltatime);
            if (glfwGetKey(window, GLFW_KEY_RIGHT)== GLFW_PRESS)
                light->UpdateX(deltatime);
            if (glfwGetKey(window, GLFW_KEY_UP)== GLFW_PRESS)
                light->UpdateY(deltatime);
            if (glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS)
                light->UpdateY(-deltatime);
        }
    }
    
    void ProcessMouseMove(double xoffset,double yoffset)
    {
        if(camera)
        {
            camera->ProcessMouseMovement(xoffset, yoffset);
        }
    }
    
    void ProcessMouseScroll(double xoffset, double yoffset)
    {
        if(camera)
        {
            camera->ProcessMouseScroll(yoffset);
        }
    }
    
    
protected:
    
    void ApplyCamera(Material* mat)
    {
        if(mat && mat->shader)
        {
            camera->Attach(mat->shader);
        }
    }
    
    void ApplyCamera(Shader* shader)
    {
        camera->Attach(shader);
    }

    void RenderQuad(GLuint map)
    {
        debugShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map);
        DrawQuad();
    }
    
    void CulLightMatrix(float n, float f, int indx=0)
    {
        mat4 camInv = glm::inverse(camera->GetViewMatrix());
        mat4 lightView = light->getViewMatrix();
        float aspect = SCR_WIDTH / (float)SCR_HEIGHT;
        float tanFov = tan(glm::radians(camera->FOV/2.0));
        float yn = n * tanFov;
        float xn = yn * aspect;
        float yf = f * tanFov;
        float xf = yf * aspect;
        vec4 frustumCorners[] = {
            vec4(xn,   yn, -n, 1.0),
            vec4(-xn,  yn, -n, 1.0),
            vec4(xn,  -yn, -n, 1.0),
            vec4(-xn, -yn, -n, 1.0),
            
            vec4(xf,   yf, -f, 1.0),
            vec4(-xf,  yf, -f, 1.0),
            vec4(xf,  -yf, -f, 1.0),
            vec4(-xf, -yf, -f, 1.0)
        };
        float max = 1e16;
        float minX = max;
        float maxX = -max;
        float minY = max;
        float maxY = -max;
        float minZ = max;
        float maxZ = -max;
        loop0j(NUM_FRUSTUM_CORNERS) {
            vec4 world = camInv * frustumCorners[j];
            vec4 light = lightView * world;
            minX = min(minX, light.x);
            maxX = max(maxX, light.x);
            minY = min(minY, light.y);
            maxY = max(maxY, light.y);
            minZ = min(minZ, light.z);
            maxZ = max(maxZ, light.z);
        }
        mat4 proj = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        lightMatrix[indx] = proj * lightView;
    }

private:
    
    void RenderShadow()
    {
        debugShader->use();
        debugShader->setFloat("near_plane", near_plane);
        debugShader->setFloat("far_plane", far_plane);
        if(debugTexID) RenderQuad(debugTexID);
    }
    
    void DrawQuad()
    {
        glBindVertexArray(quadVAO);
        glDrawArrays(DRAW_MODE, 0, 6);
        glBindVertexArray(0);
    }
    
    
public:
    Shadow* shadow = nullptr;
    
protected:
    Camera* camera = nullptr;
    Light*  light = nullptr;
    Skybox* skybox = nullptr;
    
    // shadow reinference
    bool debug;
    GLuint quadVAO, quadVBO, debugTexID = 0;
    uint SHADOW_WIDTH = 1024;
    mat4 lightMatrix[CASCACDE_NUM];
    float near_plane = 0.2f, far_plane = 7.5f;
    Shader* debugShader = nullptr;

    float timeValue;
};


#endif /* scene_h */
