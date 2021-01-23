//
// Created by adria on 15/02/2018.
//

#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <game-engine/internal/utils.hpp>
namespace GameEngine {
namespace Internal {
    template<typename T>
    void Shader::setUniform(const char *uniformName, T value) const {
        glUniform1i(glGetUniformLocation(programId_, uniformName), value);
        CheckGlError();
    }

template<>
    void Shader::setUniform(const char *uniformName, bool value) const {
        glUniform1i(glGetUniformLocation(programId_, uniformName), (int) value);
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, int value) const {
        glUniform1i(glGetUniformLocation(programId_, uniformName), value);
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, float value) const {
        glUniform1f(glGetUniformLocation(programId_, uniformName), value);
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::vec2 value) const {
        glUniform2fv(glGetUniformLocation(programId_, uniformName), 1, glm::value_ptr(value));
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(programId_, uniformName), 1, glm::value_ptr(value));
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::vec4 value) const {
        glUniform4fv(glGetUniformLocation(programId_, uniformName), 1, glm::value_ptr(value));
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::mat2 value) const {
        glUniformMatrix2fv(glGetUniformLocation(programId_, uniformName), 1, GL_FALSE, glm::value_ptr(value));
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::mat3 value) const {
        glUniformMatrix3fv(glGetUniformLocation(programId_, uniformName), 1, GL_FALSE, glm::value_ptr(value));
        CheckGlError();
    }

    template<>
    void Shader::setUniform(const char *uniformName, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(programId_, uniformName), 1, GL_FALSE, glm::value_ptr(value));
        CheckGlError();
    }

    void Shader::printShaderLog(GLuint shader) {
        //Make sure name is shader
        if (glIsShader(shader)) {
            //Shader log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            //Get info string length
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            //Allocate string
            char *infoLog = new char[maxLength];

            //Get info log
            glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
            if (infoLogLength > 0) {
                //Print Log
                printf("%s\n", infoLog);
            }

            //Deallocate string
            delete[] infoLog;
        } else {
            printf("Name %d is not a shader\n", shader);
        }

        throw std::runtime_error("Unable to compile vertex shader " + std::to_string(shader));
    }


    void Shader::buildVertexShader() {
        //Create vertex shader
        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);

        //Get vertex source
        const GLchar *vertexShaderSource[] = {vertexShaderSource_};

        //Set vertex source
        glShaderSource(vertexShader_, 1, vertexShaderSource, NULL);

        //Compile vertex source
        glCompileShader(vertexShader_);

        //Check vertex shader for errors
        GLint vShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &vShaderCompiled);
        if (vShaderCompiled != GL_TRUE) {
            printShaderLog(vertexShader_);
        }
        CheckGlError();
    }

    void Shader::buildFragmentShader() {
//Create fragment shader
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);

        //Get fragment source
        const GLchar *fragmentShaderSource[] = {fragmentShaderSource_};

        //Set fragment source
        glShaderSource(fragmentShader_, 1, fragmentShaderSource, NULL);

        //Compile fragment source
        glCompileShader(fragmentShader_);

        //Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE) {
            printShaderLog(fragmentShader_);
        }
        CheckGlError();
    }

    void Shader::build() {
        programId_ = glCreateProgram();
        buildVertexShader();
        //Attach vertex shader to program
        glAttachShader(programId_, vertexShader_);
        buildFragmentShader();
        //Attach fragment shader to program
        glAttachShader(programId_, fragmentShader_);

        //Link program
        glLinkProgram(programId_);
        //Check for errors
        GLint programSuccess = GL_TRUE;
        glGetProgramiv(programId_, GL_LINK_STATUS, &programSuccess);
        if (programSuccess != GL_TRUE)
            throw std::runtime_error("Error linking program " + std::to_string(programId_));

        //Generate the dummy texture
        GLubyte white[4] = {255, 255, 255, 255};
        glGenTextures(1, &dummyTextureID_);
        glBindTexture(GL_TEXTURE_2D, dummyTextureID_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);
        CheckGlError();
    }

    void Shader::bind() {
        //Bind program to gl context
        glUseProgram(programId_);
        glBindTexture(GL_TEXTURE_2D, dummyTextureID_);
        setUniform("Color", glm::vec4(1, 1, 1, 1));
        mode_ = GL_TRIANGLES;
        CheckGlError();
    }

    void Shader::draw() const {

        GLsizei stride = (attributes.at(Attributes::UV) ? 5 : 3) * sizeof(GLfloat);

        glBindBuffer(GL_ARRAY_BUFFER, attributes.at(Attributes::Vertex));

        //Set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) nullptr);
        glEnableVertexAttribArray(0);

        CheckGlError();

        //Set the uv
        if (attributes.at(Attributes::UV)) {
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        CheckGlError();
        //set indices
        if (attributes.count(Attributes::Indices) && mode_ == GL_TRIANGLES) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.at(Attributes::Indices));
            CheckGlError();

            int size = 0;
            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
            glDrawElements(mode_, size, GL_UNSIGNED_INT, 0);
            CheckGlError();
        } else {
            int size = 0;

            glBindBuffer(GL_ARRAY_BUFFER, attributes.at(Attributes::Vertex));
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
            glDrawArrays(mode_,0,size/stride);
        }

        //Disable vertex position
        glDisableVertexAttribArray(0);
        //Disable uv
        if (attributes.at(Attributes::UV))
            glDisableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        CheckGlError();
    }

    void Shader::unbind() {
        clear();
        //Unbind program
        glUseProgram(NULL);
        CheckGlError();
    }

    void Shader::setAttribute(Shader::Attributes attribute, GLuint glPointer) {
        attributes[attribute] = glPointer;
    }


    Shader::Shader(const char *name) : name_(name),
                                       mode_(GL_TRIANGLES){
    }

    const char *Shader::getName() const {
        return name_;
    }

    void Shader::clear() {
        attributes.clear();
        glBindTexture(GL_TEXTURE_2D, dummyTextureID_);
        setUniform("Color", glm::vec4(1, 1, 1, 1));
        CheckGlError();
    }

    void Shader::setElementMode(GLenum mode) {
        mode_ = mode;
    }

GLenum Shader::getElementMode() const
{
    return mode_;
}
}
}