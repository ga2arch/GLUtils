//
//  GLUtils.h
//  GLUtils
//
//  Created by Gabriele Carrettoni on 16/11/14.
//  Copyright (c) 2014 Gabriele Carrettoni. All rights reserved.
//

#ifndef __GLUtils__GLUtils__
#define __GLUtils__GLUtils__

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gl/glew.h"
#include "GLFW/glfw3.h"

using shaders = std::pair<GLuint, GLuint>;

class GLUtils {
    
public:
    
    static bool create_window(const char* title,
                              int w,
                              int h,
                              GLFWwindow*& win);
    
    static shaders
        compile_shaders(const std::string& vertex,
                        const std::string& fragment);
    
    static shaders
        compile_shaders_from_file(const std::string& vertex,
                                  const std::string& fragment);
    
    static bool link_shaders(shaders& shaders, GLuint& program);
    
};

#endif /* defined(__GLUtils__GLUtils__) */
