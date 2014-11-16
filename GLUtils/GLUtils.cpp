//
//  GLUtils.cpp
//  GLUtils
//
//  Created by Gabriele Carrettoni o/Users/Gabriele/Dropbox/Progetti/c/Shadertoy/Shadertoy/Shadertoy.cppn 16/11/14.
//  Copyright (c) 2014 Gabriele Carrettoni. All rights reserved.
//

#include "GLUtils.h"

bool GLUtils::create_window(const char* title, int w, int h, GLFWwindow*& win) {
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << description << std::endl;
    });
    
    if (!glfwInit()) {
        std::cout << "Error init" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    win = glfwCreateWindow(w, h, title, nullptr, nullptr);
    
    if (!win) {
        std::cout << "Error creatin window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(win);
    glewExperimental = GL_TRUE;
    glewInit();
    
    const auto renderer = glGetString(GL_RENDERER);
    const auto version  = glGetString(GL_VERSION);
    
    printf("Renderer: %s\n", renderer);
    printf("Version: %s\n", version);
    
    return true;
}

shaders GLUtils::compile_shaders(const std::string &vertex,
                                 const std::string &fragment) {
    
    const auto vertex_source = vertex.c_str();
    const auto fragment_source = fragment.c_str();
    
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader);
    
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader);
    
    return std::make_pair(vertex_shader, fragment_shader);
}

shaders GLUtils::compile_shaders_from_file(const std::string &vertex,
                                           const std::string &fragment) {
    
    std::ifstream vertex_source(vertex, std::ios::in);
    std::ifstream fragment_source(fragment, std::ios::in);
    std::stringstream v, s;

    v << vertex_source.rdbuf();
    s << fragment_source.rdbuf();
    
    return compile_shaders(v.str(), s.str());
}

bool GLUtils::link_shaders(GLuint& program, shaders& shaders) {
    auto vertex = std::get<0>(shaders);
    auto fragment = std::get<1>(shaders);
    
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    auto params = -1;
    glGetProgramiv (program, GL_LINK_STATUS, &params);
    
    if (GL_TRUE != params) {
        std::cout << "ERROR: could not link shader program" << std::endl;
        
        auto error_len = 0;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &error_len);
        
        if (error_len > 0) {
            auto max_length = error_len;
            auto actual_length = 0;
            char log[max_length];
            
            glGetShaderInfoLog (vertex, max_length, &actual_length, log);
            std::cout << "=== VERTEX SHADER ===\n"
            << log << "\n==========\n" << std::endl;
        }
        
        error_len = 0;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &error_len);
        
        if (error_len > 0) {
            auto max_length = error_len;
            auto actual_length = 0;
            char log[max_length];
            
            glGetShaderInfoLog (fragment, max_length, &actual_length, log);
            std::cout << "=== FRAGMENT SHADER ===\n"
            << log << "\n==========\n" << std::endl;
        }
        
        return false;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return true;
}