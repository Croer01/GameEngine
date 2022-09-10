//
// Created by adria on 20/08/2022.
//

#ifndef GAMEENGINE_SHADERVERSION_HPP
#define GAMEENGINE_SHADERVERSION_HPP

#include <string>
#include <stdexcept>

namespace GameEngine {
namespace Internal {
enum class ShaderVersion{
    UNKNOWN,
    V120,
    V330
};

inline std::string getShaderVersionDeclaration(ShaderVersion version){
    if (version == ShaderVersion::V120)
        return "#version 120";
    else if (version == ShaderVersion::V330)
        return "#version 330 core";
    
    throw std::runtime_error("Unsupported shader version");
};

 }
 }
#endif //GAMEENGINE_SHADERVERSION_HPP