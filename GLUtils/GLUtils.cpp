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
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
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

Shaders GLUtils::compile_shaders(const std::string &vertex,
                                 const std::string &fragment) {
    
    const auto vertex_source = vertex.c_str();
    const auto fragment_source = fragment.c_str();
    
    auto vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
    auto fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
    
    return std::make_pair(vertex_shader, fragment_shader);
}

Shaders GLUtils::compile_shaders_from_file(const std::string &vertex,
                                           const std::string &fragment) {
    
    std::ifstream vertex_source(vertex, std::ios::in);
    std::ifstream fragment_source(fragment, std::ios::in);
    std::stringstream v, s;

    v << vertex_source.rdbuf();
    s << fragment_source.rdbuf();
    
    return compile_shaders(v.str(), s.str());
}

GLuint GLUtils::compile_shader(const std::string& source,
                               GLenum type) {
    
    const auto c_source = source.c_str();

    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &c_source, nullptr);
    glCompileShader(shader);
    
    return shader;
}

GLuint GLUtils::compile_shader_from_file(const std::string& source,
                                          GLenum type) {
    
    std::ifstream shader_source(source, std::ios::in);
    std::stringstream s;

    s << shader_source.rdbuf();
    
    return compile_shader(s.str(), type);
}

bool GLUtils::link_shaders(Shaders& shaders, GLuint& program) {
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

GLuint GLUtils::make_vbo(GLenum target,
                         const float* buffer_data,
                         GLsizei buffer_size) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

GLuint GLUtils::make_vao(GLenum target,
                         GLuint vbo) {
    GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glBindBuffer (target, vbo);
    
    return vao;
}

GLuint GLUtils::make_ibo(const GLushort* indices,
                         GLsizei indices_size) {
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices_size,
                 indices, GL_STATIC_DRAW);

    return ibo;
}

void GLUtils::bind_vao(GLuint attrib,
                       int size,
                       int stride,
                       int offset) {
    
    glVertexAttribPointer (attrib,
                           size,
                           GL_FLOAT,
                           GL_FALSE,
                           stride,
                           reinterpret_cast<void*>(offset));
}

GLuint GLUtils::load_texture(const std::string& filename) {
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    
    if (error) {
        throw std::runtime_error(lodepng_error_text(error));
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    
    //When texture area is small, bilinear filter the closest mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    //When large, bilinear filter the FIRST mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
    return texture;
}
