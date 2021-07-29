//
// Created by federico on 29/07/2021.
//
#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>

class TextureImage {
public:
    unsigned int textureId;

    explicit TextureImage(std::string& filename);
};
