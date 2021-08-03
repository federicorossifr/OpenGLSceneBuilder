//
// Created by federico on 03/08/2021.
//

#include <RenderableObject.h>

RenderableObject::RenderableObject(std::string &&vertexFile, std::string &&fragmentFile, std::vector<Vertex> &&vertices,
                                   std::vector<unsigned int> &&indices) {
    vertexHandler = new VertexHandler(std::move(vertices),std::move(indices));
    shaderHandler = new ShaderHandler(std::move(vertexFile),std::move(fragmentFile));
}


void RenderableObject::setMaterial(Material &mat) {
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    shaderHandler->setVec3Uniform("material.ambient", mat.ambient);
    shaderHandler->setVec3Uniform("material.diffuse", mat.diffuse);
    shaderHandler->setVec3Uniform("material.specular", mat.specular);
    shaderHandler->setScalarUniform("material.shininess", mat.shininess);
}

void RenderableObject::setIllumination(LightProperties &light, const std::string &lightPropertyName) {
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    shaderHandler->setVec3Uniform(lightPropertyName+".ambient", light.ambient);
    shaderHandler->setVec3Uniform(lightPropertyName+".diffuse", light.diffuse);
    shaderHandler->setVec3Uniform(lightPropertyName+".specular", light.specular);
}

void RenderableObject::setDirectionalLight(DirectionalLight &dir) {
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    setIllumination(dir.properties,"dirLight");
    shaderHandler->setVec3Uniform("dirLight.direction",dir.direction);
}

void RenderableObject::setFlashLight(FlashLight &fl){
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    setIllumination(fl.properties,"flashLight");
    shaderHandler->setVec3Uniform("flashLight.position",fl.position);
    shaderHandler->setVec3Uniform("flashLight.direction",fl.direction);
    shaderHandler->setScalarUniform("flashLight.cutOff",fl.cutOff);
    shaderHandler->setScalarUniform("flashLight.outerCutOff",fl.outerCutOff);
}

void RenderableObject::addPointLight(PointLight &pl) {
    if(spotLightIdx == maxSpotLights-1) return;
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    std::string baseName = "pointLights[" + std::to_string(spotLightIdx++) + "]";
    setIllumination(pl.properties,baseName);
    shaderHandler->setScalarUniform("numPointLights",(int)spotLightIdx); // Already incremented before ;)
    shaderHandler->setVec3Uniform(baseName+".position",     pl.position);
    shaderHandler->setScalarUniform(baseName+".constant",   pl.constant);
    shaderHandler->setScalarUniform(baseName+".linear",     pl.linear);
    shaderHandler->setScalarUniform(baseName+".quadratic",  pl.quadratic);
}

void RenderableObject::setTextureMaterial(Material &mat) {
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    shaderHandler->setScalarUniform("material.diffuse", 0); // GLTEXTURE0 for Diffuse Map
    shaderHandler->setScalarUniform("material.specular", 1); // GLTEXTURE1 for Specular Map
    shaderHandler->setScalarUniform("material.shininess", mat.shininess);
    shaderHandler->setScalarUniform("material.canEmit",false);

}

void RenderableObject::enableNormalAttribute() {
    vertexHandler->bindNormal();
}

void RenderableObject::enableEmissionMap(){
    if(shaderHandler == nullptr) return;
    shaderHandler->useShader();
    shaderHandler->setScalarUniform("material.emission", 2); // GLTEXTURE2 for emission Map
    shaderHandler->setScalarUniform("material.canEmit",true);
}

void RenderableObject::setTexture(std::string &&filename, bool hasAlpha) {
    vertexHandler->bindTexture(std::move(filename),hasAlpha);
}