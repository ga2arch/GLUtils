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
#include "lodepng.h"

using Shaders = std::pair<GLuint, GLuint>;

class GLUtils {
    
public:
    
    static bool create_window(const char* title,
                              int w,
                              int h,
                              GLFWwindow*& win);
    
    static GLuint compile_shader(const std::string& source,
                                 GLenum type);
    
    static GLuint compile_shader_from_file(const std::string& source,
                                           GLenum type);
    
    static Shaders
        compile_shaders(const std::string& vertex,
                        const std::string& fragment);
    
    static Shaders
        compile_shaders_from_file(const std::string& vertex,
                                  const std::string& fragment);
    
    static bool link_shaders(Shaders& shaders, GLuint& program);
    
    static GLuint make_vbo(GLenum target,
                           const GLfloat* buffer_data,
                           GLsizei buffer_size);
    
    static GLuint make_vao(GLenum target,
                           GLuint vbo);
    
    static GLuint make_ibo(const GLushort* indices,
                           GLsizei indices_size);
    
    static void bind_vao(GLuint attrib,
                         int size,
                         int stride,
                         int offset = 0);
    
    static GLuint load_texture(const std::string& filename);
};

#endif /* defined(__GLUtils__GLUtils__) */
