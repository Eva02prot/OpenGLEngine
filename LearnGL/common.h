//
//  common.h
//  LearnGL
//
//  Created by 彭怀亮 on 5/16/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef common_h
#define common_h

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include<stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

#include "std/const.h"


#define DeclareSington(TYPE)  \
private:    \
    TYPE() { }  \
    TYPE(const TYPE &); \
    TYPE& operator=(const TYPE &);  \
    static TYPE instance;   \
public: \
    static TYPE* getInstance() { return &instance; } \


#define MAX(a, b) ((a) > (b) ? (a):(b))

#endif /* common_h */
