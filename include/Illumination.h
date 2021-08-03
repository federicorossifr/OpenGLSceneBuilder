//
// Created by federico on 03/08/2021.
//
#pragma once

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirectionalLight {
    glm::vec3 direction;
    LightProperties properties;
};

struct PointLight {
    glm::vec3 position;

    LightProperties properties;

    float constant;
    float linear;
    float quadratic;
};

struct FlashLight {
    glm::vec3 position;
    glm::vec3 direction;
    LightProperties properties;

    float cutOff;
    float outerCutOff;

    bool state;
};