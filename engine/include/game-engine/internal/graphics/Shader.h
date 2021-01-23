//
// Created by adria on 15/02/2018.
//

#ifndef GAMEENGINE_SHADER_H
#define GAMEENGINE_SHADER_H

#include <string>
#include <map>
#include <memory>
#include <GL/glew.h>
namespace GameEngine {
namespace Internal {
    class Shader {
    public:
        enum class Attributes {
            Vertex,
            Indices,
            UV
        };
    private:
        const char *name_;
        const char *vertexShaderSource_;
        const char *fragmentShaderSource_;
        GLuint programId_;
        GLuint dummyTextureID_;
        GLuint vertexShader_;
        GLuint fragmentShader_;
        GLenum mode_;
        std::map<Attributes, unsigned int> attributes;

        void buildVertexShader();

        void buildFragmentShader();

        void printShaderLog(GLuint shader);

    public:
        Shader(const char *name);

        void setVertexShader(const char *vertexShader) { vertexShaderSource_ = vertexShader; };

        void setFragmentShader(const char *fragmentShader) { fragmentShaderSource_ = fragmentShader; };

        void setElementMode(GLenum mode);
        GLenum getElementMode() const;

        void setAttribute(Shader::Attributes attribute, GLuint glPointer);

        template<typename T>
        void setUniform(const char *uniformName, T value) const;

        const char *getName() const;

        void build();

        void draw() const;

        void bind();

        void unbind();

        void clear();
    };

    typedef std::shared_ptr<Shader> ShaderRef;
}
}
#endif //GAMEENGINE_SHADER_H
