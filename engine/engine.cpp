//
//  common.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "engine.h"
#include <sys/timeb.h>
#include "std/util.hpp"

namespace engine
{

    uint SCR_WIDTH = 800;
    uint SCR_HEIGHT = 600;
    
    uint UI_WIDTH = 800;
    uint UI_HEIGHT = 600;

    uint RENDER_WIDTH = 1600;
    uint RENDER_HEIGTH = 1200;
    uint DRAW_MODE = GL_TRIANGLES;
    bool ENG_PAUSE = false;
    
    iScene* scene = nullptr;
    MeshData* plane = nullptr;
    MeshData* cube = nullptr;
    MeshData* quad = nullptr;
    MeshData* quad2 = nullptr;
    uint *spere_indices = nullptr;
    float *spere_vertices = nullptr;
    time_t start_time;
    float deltatime;
    
    void SystemInfo(int level)
    {
#ifdef DEBUG
        std::cout<<"VENDOR:    "<< glGetString(GL_VENDOR)<<std::endl;
        std::cout<<"RENDERER:  "<< glGetString(GL_RENDERER)<<std::endl;
        std::cout<<"VERSION:   "<< glGetString(GL_VERSION)<<std::endl;
        std::cout<<"GLSL VER:  "<< glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
        if(level > 0)
        {
            int ext=0; glGetIntegerv(GL_NUM_EXTENSIONS,&ext);
            std::cout<<"EXTENSIONS "<< ext<<std::endl;
            loop(ext) std::cout<<i<<" "<<glGetStringi(GL_EXTENSIONS,i)<<std::endl;
            if(level > 2)
            {
                int num_formats;
                glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_formats);
                std::cout<<"Texture extensions: "<<num_formats<<std::endl;
                int *formats = (int*)alloca(num_formats * sizeof(int));
                glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
                loop(num_formats) std::cout<<i<<" 0x"<<hex<<formats[i]<<dec<<std::endl;
            }
        }
#endif
    }
    
    float GetRuntime()
    {
        timeb t;
        ftime(&t);
        if (start_time == 0) start_time = t.time;
        return t.time - start_time + t.millitm * 0.001f;
    }

    float GetDeltaTime()
    {
        return deltatime;
    }
    
    void OnApplicationQuit()
    {
        std::cout<<"engine will be quit"<<std::endl;
    }
    
    void OnApplicationPause(bool pause)
    {
        std::cout<<"engine pause "<<pause<<std::endl;
        ENG_PAUSE = pause;
    }

    void* InitPlane(GLuint *vao, GLuint *vbo, Shader* shader)
    {
        if(plane==nullptr) plane = ReadMesh("plane","common");
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glBindVertexArray(*vao);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        plane->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) plane->BindVert(shader);
        return plane;
    }

    void* InitCube(GLuint *vao, GLuint *vbo, Shader* shader)
    {
        if(cube == nullptr) cube = ReadMesh("cube","common");
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBindVertexArray(*vao);
        cube->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) cube->BindVert(shader);
        return cube;
    }

    // ndc position -> right_btm corner
    void* InitQuad(GLuint *vao, GLuint *vbo, Shader* shader)
    {
        if(quad == nullptr) quad = ReadMesh("quad","common");
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glBindVertexArray(*vao);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        quad->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) quad->BindVert(shader);
        return quad;
    }
    
    // ndc position->[-1,1]
    void* InitFullQuad(GLuint *vao, GLuint *vbo, Shader* shader)
    {
        if(quad2 == nullptr) quad2 = ReadMesh("quad2","common");
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glBindVertexArray(*vao);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        quad2->ConfigAttribute();
        glBindVertexArray(0);
        if(shader) quad2->BindVert(shader);
        return quad2;
    }
    
    uint InitSpere(GLuint *vao, GLuint *vbo, GLuint *ebo)
    {
        const uint X_SEGMENTS = 32;
        const uint Y_SEGMENTS = 32;
        uint size = (X_SEGMENTS + 1) * Y_SEGMENTS * 2;
        if(spere_indices == nullptr || spere_vertices == nullptr)
        {
            std::vector<glm::vec3> positions;
            std::vector<glm::vec2> uv;
            std::vector<glm::vec3> normals;
            bool oddRow = false;
            for (uint y = 0; y <= Y_SEGMENTS; ++y)
            {
                for (uint x = 0; x <= X_SEGMENTS; ++x)
                {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    float yPos = std::cos(ySegment * PI);
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    
                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }
            spere_indices=new uint[size];
            int idx = 0;
            for (int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        *(spere_indices + idx++) = y * (X_SEGMENTS + 1) + x;
                        *(spere_indices + idx++) = (y + 1) * (X_SEGMENTS + 1) + x;
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        *(spere_indices + idx++) = (y + 1) * (X_SEGMENTS + 1) + x;
                        *(spere_indices + idx++) = y * (X_SEGMENTS + 1) + x;
                    }
                }
                oddRow = !oddRow;
            }

            idx = 0;
            spere_vertices = new float[positions.size() * 8];
            for (int i = 0; i < positions.size(); ++i)
            {
                *(spere_vertices + idx++) = positions[i].x;
                *(spere_vertices + idx++) = positions[i].y;
                *(spere_vertices + idx++) = positions[i].z;
                *(spere_vertices + idx++) = uv[i].x;
                *(spere_vertices + idx++) = uv[i].y;
                *(spere_vertices + idx++) = normals[i].x;
                *(spere_vertices + idx++) = normals[i].y;
                *(spere_vertices + idx++) = normals[i].z;
            }
        }
        float stride = (3 + 2 + 3) * sizeof(float);
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glGenBuffers(1, ebo);
        glBindVertexArray(*vao);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, (X_SEGMENTS+1) *(Y_SEGMENTS+1) *stride, spere_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint), spere_indices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return size;
    }
    
    
    void SetPosition(glm::mat4& mat, glm::vec3& pos)
    {
        mat[3][0] = pos.x;
        mat[3][1] = pos.y;
        mat[3][2] = pos.z;
    }
    
    void error_stop(bool condition, const char* msg)
    {
        if (!condition) {
            std::cerr<<msg<<std::endl;
        }
        assert(condition);
    }

    std::string Macro(const char* k1)
    {
        std::stringstream stream;
        MACRO(k1, NULL)
        return stream.str();
    }

    std::string Macro(const char* k1, const char* v1)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        return stream.str();
    }

    std::string Macro(const char* k1, const char* v1, const char* k2, const char* v2)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        MACRO(k2, v2)
        return stream.str();
    }

    std::string Macro(const char* k1, const void* v1, const char* k2, const void* v2, const char* k3, const char* v3)
    {
        std::stringstream stream;
        MACRO(k1, v1)
        MACRO(k2, v2)
        MACRO(k3, v3)
        return stream.str();
    }

    void SetRenderMode(const uint mode)
    {
        DRAW_MODE = mode;
    }

    void SetWindowSize(const int width, const int height)
    {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
#ifdef _GLES_
        RENDER_WIDTH = width;
        RENDER_HEIGTH = height;
#else
        RENDER_WIDTH = width * 2;
        RENDER_HEIGTH = height * 2;
#endif
    }
    
    uint Hash(const std::string str)
    {
        if (str.empty()) return 0;
        uint hash = 0;
        int seed = 5;
        loop(str.size()) hash = (hash << seed) + str[i] + hash;
        return hash;
    }

    void caltangent(const glm::vec3 pos1, const glm::vec3 pos2, const glm::vec3 pos3,
                    const glm::vec2 uv1, const glm::vec2 uv2, const glm::vec2 uv3,
                    glm::vec3* tan, glm::vec3* bit)
    {
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        
        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        (*tan).x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        (*tan).y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        (*tan).z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        (*tan) = normalize(*tan);
        
        (*bit).x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        (*bit).y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        (*bit).z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        (*bit) = glm::normalize(*bit);
    }
        
}
