#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "vokra.h"

// === Vertex/Fragment shaders ===
// Shape shaders 
static const char* shapeVertexShaderSrc = "#version 330 core\n"
"layout(location = 0) in vec2 aPos;\n"
"uniform vec2 uOffset;\n"
"uniform vec2 uScale;\n"
"uniform vec2 uResolution;\n"
"void main() {\n"
"  vec2 pixelPos = uOffset + aPos * uScale;\n"
"  vec2 ndc = (pixelPos / uResolution) * 2.0 - 1.0;\n"
"  gl_Position = vec4(ndc, 0.0, 1.0);\n"
"}\n";

static const char* shapeFragmentShaderSrc = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 uColor;\n"
"void main() {\n"
"  FragColor = vec4(uColor, 1.0);\n"
"}\n";

// Texture shaders
static const char* texVertexShaderSrc = "#version 330 core\n"
"layout(location = 0) in vec2 aPos;\n"
"layout(location = 1) in vec2 aTex;\n"
"out vec2 TexCoord;\n"
"uniform vec2 uOffset;\n"
"uniform vec2 uScale;\n"
"uniform vec2 uResolution;\n"
"void main() {\n"
"  vec2 pixelPos = uOffset + aPos * uScale;\n"
"  vec2 ndc = (pixelPos / uResolution) * 2.0 - 1.0;\n"
"  gl_Position = vec4(ndc, 0.0, 1.0);\n"
"  TexCoord = aTex;\n"
"}\n";

static const char* texFragmentShaderSrc = "#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex;\n"
"void main() {\n"
"  FragColor = texture(tex, TexCoord);\n"
"}\n";

// Shader Compilers for Shapes Renderering
static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    GLint success;
    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if(!success){
        char info[512];
        glGetShaderInfoLog(s,512,NULL,info);
        fprintf(stderr,"Shader compile error: %s\n",info);
        exit(1);
    }
    return s;
}

static GLuint createProgram(const char* vertSrc, const char* fragSrc){
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if(!success){
        char info[512];
        glGetProgramInfoLog(prog,512,NULL,info);
        fprintf(stderr,"Program link error: %s\n",info);
        exit(1);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}

// Shape Rendering begin
void vk_begin(VkState* state){
    state->shaderProgram = createProgram(shapeVertexShaderSrc, shapeFragmentShaderSrc);

    glGenVertexArrays(1,&state->vao);
    glGenBuffers(1,&state->vbo);

    glBindVertexArray(state->vao);
    glBindBuffer(GL_ARRAY_BUFFER, state->vbo);

    // For shapes, only pos needed (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

// Rectange
void vk_renderRect(VkState* state,float x,float y,float w,float h,Colour c){
    float vertices[] = {0,0, 1,0, 1,1, 0,1};

    glUseProgram(state->shaderProgram);
    glBindVertexArray(state->vao);
    glBindBuffer(GL_ARRAY_BUFFER,state->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    GLint uOffset = glGetUniformLocation(state->shaderProgram,"uOffset");
    GLint uScale  = glGetUniformLocation(state->shaderProgram,"uScale");
    GLint uColor  = glGetUniformLocation(state->shaderProgram,"uColor");
    GLint uRes    = glGetUniformLocation(state->shaderProgram,"uResolution");

    glUniform2f(uOffset,x,y);
    glUniform2f(uScale,w,h);
    glUniform2f(uRes,(float)state->screenWidth,(float)state->screenHeight);
    glUniform3f(uColor,c.r,c.g,c.b);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Base triangle like Equalateral
void vk_renderETriangle(VkState* state,float x,float y,float w,float h,Colour c){
    float vertices[] = {0.5f,1, 0,0, 1,0};

    glUseProgram(state->shaderProgram);
    glBindVertexArray(state->vao);
    glBindBuffer(GL_ARRAY_BUFFER,state->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    GLint uOffset = glGetUniformLocation(state->shaderProgram,"uOffset");
    GLint uScale  = glGetUniformLocation(state->shaderProgram,"uScale");
    GLint uColor  = glGetUniformLocation(state->shaderProgram,"uColor");
    GLint uRes    = glGetUniformLocation(state->shaderProgram,"uResolution");

    glUniform2f(uOffset,x,y);
    glUniform2f(uScale,w,h);
    glUniform2f(uRes,(float)state->screenWidth,(float)state->screenHeight);
    glUniform3f(uColor,c.r,c.g,c.b);

    glDrawArrays(GL_TRIANGLES,0,3);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// define own vertices triangle (any shape with 3 vertices - thus a triangl)
void vk_renderSpTriangle(VkState* state,float x1,float y1,float x2,float y2,float x3,float y3,Colour c){
    float vertices[] = {x1,y1,x2,y2,x3,y3};

    glUseProgram(state->shaderProgram);
    glBindVertexArray(state->vao);
    glBindBuffer(GL_ARRAY_BUFFER,state->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    GLint uColor = glGetUniformLocation(state->shaderProgram,"uColor");
    GLint uRes   = glGetUniformLocation(state->shaderProgram,"uResolution");
    GLint uOffset= glGetUniformLocation(state->shaderProgram,"uOffset");
    GLint uScale = glGetUniformLocation(state->shaderProgram,"uScale");

    glUniform3f(uColor,c.r,c.g,c.b);
    glUniform2f(uRes,(float)state->screenWidth,(float)state->screenHeight);
    glUniform2f(uOffset,0,0);
    glUniform2f(uScale,1,1);

    glDrawArrays(GL_TRIANGLES,0,3);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// === Cleanup === //
void vk_end(VkState* state){
    glDeleteProgram(state->shaderProgram);
    glDeleteBuffers(1,&state->vbo);
    glDeleteVertexArrays(1,&state->vao);
}

// === Texture loading === //
GLuint createGLTexture(Texture tex){
    GLuint texID;
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    GLenum fmt = (tex.channels==4)?GL_RGBA:GL_RGB;
    glTexImage2D(GL_TEXTURE_2D,0,fmt,tex.width,tex.height,0,fmt,GL_UNSIGNED_BYTE,tex.data);
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(tex.data); // free CPU memory immediately
    return texID;
}

GLuint vk_loadTexture(const char* filepath){
    Texture tex = {0};
    tex.data = stbi_load(filepath,&tex.width,&tex.height,&tex.channels,0);
    if(!tex.data){
        fprintf(stderr,"Failed to load texture: %s\n",filepath);
        exit(1);
    }
    return createGLTexture(tex);
}

// === Texture rendering === //
void vk_displayTexture(VkState* state,GLuint texID,float x,float y,float w,float h){
    // Lazy init texture shader
    static GLuint texShader = 0;
    static GLuint texVAO = 0;
    static GLuint texVBO = 0;
    if(texShader==0){
        texShader = createProgram(texVertexShaderSrc,texFragmentShaderSrc);

        // quad: pos(x,y) + tex(u,v)
				// This will still render like other shapes but i need GLebable otherwise thewre will be black space visible
        float verts[] = {
            0,0, 0,0,
            1,0, 1,0,
            1,1, 1,1,
            0,1, 0,1
        };
        glGenVertexArrays(1,&texVAO);
        glGenBuffers(1,&texVBO);
        glBindVertexArray(texVAO);
        glBindBuffer(GL_ARRAY_BUFFER,texVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    glUseProgram(texShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texID);
    glUniform1i(glGetUniformLocation(texShader,"tex"),0);

    GLint uOffset = glGetUniformLocation(texShader,"uOffset");
    GLint uScale  = glGetUniformLocation(texShader,"uScale");
    GLint uRes    = glGetUniformLocation(texShader,"uResolution");

    glUniform2f(uOffset,x,y);
    glUniform2f(uScale,w,h);
    glUniform2f(uRes,(float)state->screenWidth,(float)state->screenHeight);

    glBindVertexArray(texVAO);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glBindVertexArray(0);
    glUseProgram(0);
}

