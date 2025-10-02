#include "renderer.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>



// Simple vertex and fragment shaders
static const char* vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "uniform vec2 uOffset;\n"
    "uniform vec2 uScale;\n"
    "void main() {\n"
    "   vec2 pos = aPos * uScale + uOffset;\n"
    "   gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}";

static const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 uColor;\n"
    "void main() {\n"
    "   FragColor = vec4(uColor, 1.0);\n"
    "}";

// Compile a shader
static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        fprintf(stderr, "Shader compilation error: %s\n", info);
        exit(1);
    }
    return shader;
}

// Create shader program
static GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        fprintf(stderr, "Program linking error: %s\n", info);
        exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Initialize the renderer
void vk_begin(VkState* state) {
    state->shaderProgram = createShaderProgram();

    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &state->vao);
    glGenBuffers(1, &state->vbo);

    glBindVertexArray(state->vao);

    glBindBuffer(GL_ARRAY_BUFFER, state->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Draw a rectangle with color
void vk_renderRect(VkState* state, float x, float y, float w, float h, float r, float g, float b) {
    glUseProgram(state->shaderProgram);

    GLint uOffset = glGetUniformLocation(state->shaderProgram, "uOffset");
    GLint uScale = glGetUniformLocation(state->shaderProgram, "uScale");
    GLint uColor = glGetUniformLocation(state->shaderProgram, "uColor");

    glUniform2f(uOffset, x, y);
    glUniform2f(uScale, w, h);
    glUniform3f(uColor, r, g, b);

    glBindVertexArray(state->vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
}


// Cleanup
void vk_end(VkState* state) {
    glDeleteProgram(state->shaderProgram);
    glDeleteBuffers(1, &state->vbo);
    glDeleteVertexArrays(1, &state->vao);
}

