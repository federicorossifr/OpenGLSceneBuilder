//
// Created by federico on 24/07/2021.
//

#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class ShaderHandler {
public:
    // the program ID
    unsigned int shaderProgramId;

    ShaderHandler(std::string &&vertexFile, std::string &&fragmentFile);

    ~ShaderHandler() { glDeleteShader(shaderProgramId); }

    void useShader();

    template<typename UniformType>
    void setScalarUniform(const std::string &name, UniformType value) const;
};
