//
// Created by adria on 15/02/2018.
//

#ifndef GAMEENGINE_SHADER_HPP
#define GAMEENGINE_SHADER_HPP

#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <game-engine/internal/graphics/ShaderVersion.hpp>

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
        ShaderVersion version_;
        std::vector<std::string> vertexShaderInput_;
        std::vector<std::string> vertexShaderOutput_;
        std::string vertexShaderSource_;
        std::vector<std::string> fragmentShaderInput_;
        std::vector<std::string> fragmentShaderOutput_;
        std::string fragmentShaderSource_;
        GLuint programId_;
        GLuint dummyTextureID_;
        GLuint vertexShader_;
        GLuint fragmentShader_;
        GLenum mode_;
        std::map<Attributes, unsigned int> attributes;
        std::vector<std::string> locations_;

        void buildVertexShader();

        void buildFragmentShader();

        void printShaderLog(const char *shaderType, GLuint shader);
        void writeVertexInput(std::stringstream &stream);
        void writeVertexOutput(std::stringstream &stream);
        void writeFragmentInput(std::stringstream &stream);
        void writeFragmentOutput(std::stringstream &stream);
    public:
        Shader(const char *name, ShaderVersion version);

        ShaderVersion getGlslsVersion() const;

        void addVertexInput(const std::string &input);
        void addVertexOutput(const std::string &output);
        void setVertexShader(const std::string &vertexShader) { vertexShaderSource_ = vertexShader; };

        void addFragmentInput(const std::string &input);
        void addFragmentOutput(const std::string &output);
        void setFragmentShader(const std::string &fragmentShader) { fragmentShaderSource_ = fragmentShader; };

        void setElementMode(GLenum mode);
        GLenum getElementMode() const;

        void setAttribute(Shader::Attributes attribute, GLuint glPointer);
        
        void addBindLocation(const std::string &locationName);

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
#endif //GAMEENGINE_SHADER_HPP
