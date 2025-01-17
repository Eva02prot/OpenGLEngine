//
//  scenemgr.h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef scenemgr_h
#define scenemgr_h

#include "scene.h"
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"
#include "scene4.h"
#include "scene5.h"
#include "scene6.h"
#include "scene7.h"
#include "texmgr.hpp"

class SceneMgr : iSceneMgr
{
    DeclareSington(SceneMgr)
    
private:
    ~SceneMgr()
    {
        LeaveScene();
        SAFE_DELETE(lb_scene1);
        SAFE_DELETE(lb_scene2);
        SAFE_DELETE(lb_scene3);
        SAFE_DELETE(lb_scene4);
        SAFE_DELETE(lb_scene5);
        SAFE_DELETE(lb_scene6);
        SAFE_DELETE(lb_scene7);
        SAFE_DELETE(lb_fps);
        SAFE_DELETE(lb_copy);
    }
    
    void ClickScene(UIObject* oj)
    {
        instance.ChangeTo(TY_Scene1 + oj->uid);
    }

    
public:
    
    void Init()
    {
        ChangeTo(TY_Scene1);
        glCheckError();
        lb_scene1 = new UILabel(vec2(60,450), vec3(1), 1, "Scene1", TY_Scene1);
        lb_scene2 = new UILabel(vec2(60,405), vec3(1), 1, "Scene2", TY_Scene2);
        lb_scene3 = new UILabel(vec2(60,360), vec3(1), 1, "Scene3", TY_Scene3);
        lb_scene4 = new UILabel(vec2(60,315), vec3(1), 1, "Scene4", TY_Scene4);
        lb_scene5 = new UILabel(vec2(60,270), vec3(1), 1, "Scene5", TY_Scene5);
        lb_scene6 = new UILabel(vec2(60,225), vec3(1), 1, "Scene6", TY_Scene6);
        lb_scene7 = new UILabel(vec2(60,180), vec3(1), 1, "Scene7", TY_Scene7);
        lb_fps = new UILabel(vec2(740,580), vec3(1,0,0), 0.5f);
        lb_copy = new UILabel(vec2(20), vec3(1), 0.4f);
        auto f  = Bindfunc(SceneMgr::ClickScene);
        lb_scene1->RegistCallback(f);
        lb_scene2->RegistCallback(f);
        lb_scene3->RegistCallback(f);
        lb_scene4->RegistCallback(f);
        lb_scene5->RegistCallback(f);
        lb_scene6->RegistCallback(f);
        lb_scene7->RegistCallback(f);
    }
    
    void LeaveScene()
    {
        SAFE_DELETE(current);
    }
    
    void Draw(float delta)
    {
        if(current)
        {
            current->DrawScenes();
        }
        delay++;
        if(delay % 8 ==0)
        {
            lb_fps->setText("FPS: "+to_string_with_precision(1/delta,4));
            lb_copy->setText("@copyright (c) penghuailiang");
        }
    }
    
    bool ChangeTo(int type)
    {
        if(current && type == current->getType())
        {
            std::cout<<"You are already enter scene "<<current->getType()<<std::endl;
            return false;
        }
        else
        {
            LeaveScene();
            glCheckError();
            if(type == TY_Scene1)   current = new Scene1();
            if(type == TY_Scene2)   current = new Scene2();
            if(type == TY_Scene3)   current = new Scene3();
            if(type == TY_Scene4)   current = new Scene4();
            if(type == TY_Scene5)   current = new Scene5();
            if(type == TY_Scene6)   current = new Scene6();
            if(type == TY_Scene7)   current = new Scene7();
            if(current)
            {
                current->Initial();
                setEngineScene();
            }
            return true;
        }
    }
    
    
    void setEngineScene()
    {
        scene = (iScene*)(current);
    }
    
    void ProcessKeyboard(GLFWwindow *window, float deltatime)
    {
        if(current)
        {
            current->ProcessKeyboard(window, deltatime);
        }
    }
    
    void ProcessMouseMove(double xoffset,double yoffset)
    {
        if(current)
        {
            current->ProcessMouseMove(xoffset, yoffset);
        }
    }
    
    void ProcessMouseScroll(double xoffset,double yoffset)
    {
        if(current)
        {
            current->ProcessMouseScroll(xoffset, yoffset);
        }
    }
    
    
    
private:
    Scene *current = nullptr;
    UILabel *lb_scene1, *lb_scene2, *lb_scene3, *lb_scene4, *lb_scene5, *lb_scene6,*lb_scene7;
    UILabel *lb_fps, *lb_copy;
    uint delay;
};

#endif /* scenemgr_h */
