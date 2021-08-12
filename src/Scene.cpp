//
// Created by federico on 10/08/2021.
//

#include <Scene.h>
#include <VertexGenerators.h>

#include <string_view>

void parseVec3(simdjson::simdjson_result<simdjson::ondemand::array>& jarr,glm::vec3& vec) {
    auto arr = jarr.value();
    size_t i{0};
    for(double v: arr) vec[i++] = v;
}

void parseVec4(simdjson::simdjson_result<simdjson::ondemand::array>& jarr,glm::vec4& vec) {
    auto arr = jarr.value();
    size_t i{0};
    for(double v: arr) vec[i++] = v;
}

std::vector<std::string> parsePathList(simdjson::simdjson_result<simdjson::ondemand::array>& jarr) {
    std::vector<std::string> list;
    auto arr = jarr.value();
    for(std::string_view s: arr) list.emplace_back(s);
    return list;
}

template <class T>
void parseIlluminationProperties(simdjson::simdjson_result<T>& jprop,LightProperties& lp) {
    auto ambient = jprop["Ambient"].get_array();
    parseVec3(ambient,lp.ambient);
    auto diffuse = jprop["Diffuse"].get_array();
    parseVec3(diffuse,lp.diffuse);
    auto specular = jprop["Specular"].get_array();
    parseVec3(specular,lp.specular);
}

void parsePointLight(simdjson::simdjson_result<simdjson::ondemand::object>& jpl,PointLight& pointLight) {
    LightProperties lp{};

    auto jpl_pos = jpl["Position"].get_array();
    parseVec3(jpl_pos,pointLight.position);

    auto properties = jpl["Properties"].get_object();
    parseIlluminationProperties(properties,pointLight.properties);

    auto constant = jpl["Constant"].get_double().value();
    auto linear = jpl["Linear"].get_double().value();
    auto quadratic = jpl["Quadratic"].get_double().value();

    pointLight.constant = constant;
    pointLight.quadratic = quadratic;
    pointLight.linear = linear;
}

void parseDirectionalLight(simdjson::simdjson_result<simdjson::ondemand::object>& jdl,DirectionalLight& directionalLight) {
    // Direction
    simdjson::simdjson_result jdl_pos = jdl["Position"].get_array();

    parseVec3(jdl_pos,directionalLight.direction);

    // Properties

    auto properties = jdl["Properties"].get_object();
    parseIlluminationProperties(properties,directionalLight.properties);
}



template <class T>
void setupIlluminationFromJSON(simdjson::simdjson_result<T>& jill,Scene& scene) {


    try {
        simdjson::simdjson_result jdl = jill["DirectionalLight"].get_object();
        DirectionalLight dl{};
        parseDirectionalLight(jdl,dl);
        scene.illumination.directionalLight = dl;
    } catch (std::exception e) {}

    // PointLights
    auto jPointLights = jill["PointLights"].get_array();
    for(auto jPointLight:jPointLights) {
        auto pointLightObj = jPointLight.get_object();
        PointLight pl{};
        parsePointLight(pointLightObj,pl);
        scene.illumination.pointLights.push_back(pl);
    }
}

void parseMaterial(simdjson::simdjson_result<simdjson::ondemand::object>& jobj,Material& mat) {
    auto specular = jobj["Specular"].get_array();
    parseVec3(specular,mat.specular);

    mat.shininess = jobj["Shininess"].get_double().value();
}

inline bool isPNG(std::string const & path)
{
    const std::string suffix = ".png";
    if (suffix.size() > path.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), path.rbegin());
}



void buildRenderableObjectFromJSON(simdjson::simdjson_result<simdjson::ondemand::object>& jobj,RenderableObject& obj) {

    std::string_view shape = jobj["Shape"].get_string().value();
    glm::vec3 origin{};
    auto jorigin = jobj["Position"].get_array();
    parseVec3(jorigin,origin);
    glm::vec4 colour{};
    auto jcolor = jobj["Colour"].get_array();
    parseVec4(jcolor,colour);

    if(shape == "Cube") {
        float side = jobj["Side"].get_double().value();
        auto cubeCoords = glTests::createCubeWithNormal(origin,side,{colour,colour,colour,colour,colour,colour,colour,colour});
        obj.vertexHandler = new VertexHandler(std::move(cubeCoords.first),std::move(cubeCoords.second));
    } else if(shape == "Plane") {
        float side = jobj["Side"].get_double().value();
        auto planeCoords = glTests::createPlaneWithNormal(origin,side,{colour,colour,colour,colour,colour,colour,colour,colour});
        obj.vertexHandler = new VertexHandler(std::move(planeCoords.first),std::move(planeCoords.second));
    }

    auto jShaderList = jobj["Shaders"].get_array();
    auto shaderList = parsePathList(jShaderList);
    if(shaderList.size() == 2) obj.shaderHandler = new ShaderHandler(std::move(shaderList.at(0)),std::move(shaderList.at(1)));
    if(shaderList.size() == 3) obj.shaderHandler = new ShaderHandler(std::move(shaderList.at(0)),std::move(shaderList.at(1)),std::move(shaderList.at(2)));
    obj.objModelFun = [](float time) {return glm::mat4(1.f);};

    obj.postModelFun = [&,gobj = obj](float time,GLApplication* app) {
        gobj.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };

    auto jMaterial = jobj["Material"].get_object();
    Material material{};
    parseMaterial(jMaterial,material);
    obj.enableNormalAttribute();
    obj.setTextureMaterial(material);

    auto jTextures = jobj["Textures"].get_array();
    auto textureList = parsePathList(jTextures);
    for(auto texturePath: textureList) {
        bool isPng = isPNG(texturePath);
        obj.setTexture(std::move(texturePath),isPng);
    }

    obj.shaderHandler->setScalarUniform("shadowMap",31);
    obj.shaderHandler->setScalarUniform("depthMap",30);
}

void Scene::syncIllumination() {
    for(auto obj:objects) {
        obj.setDirectionalLight(this->illumination.directionalLight);
        obj.setPointLights(this->illumination.pointLights);
        obj.setFlashLight(this->illumination.flashLight);
    }
}

void Scene::loadSceneFromJSONFile(std::string jsonPath,Scene& scene) {
    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string::load(jsonPath);
    auto sceneDocument = parser.iterate(json);
    auto renderables = sceneDocument["Objects"].get_array();
    scene.objects.reserve(renderables.count_elements());
    for(auto obj: renderables) {
        auto _obj = obj.get_object();
        RenderableObject robj{};
        buildRenderableObjectFromJSON(_obj,robj);
        auto ref = scene.objects.emplace_back(robj);
        try {
            auto key = std::string(_obj["Key"].get_string().value());
            std::cout << key << std::endl;
            std::cout << &scene.objects.back() << std::endl;
            scene.objectDOM[key] = &scene.objects.back();
        } catch (std::exception e) {}
    }
    auto illumination = sceneDocument["Illumination"].get_object();
    setupIlluminationFromJSON(illumination,scene);
}

