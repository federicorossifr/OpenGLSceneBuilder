//
// Created by federico on 10/08/2021.
//

#include <Scene.h>
#include <VertexGenerators.h>



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

std::vector<std::string> parseShaderList(simdjson::simdjson_result<simdjson::ondemand::array>& jarr) {
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



template <class T>
void setupIlluminationFromJSON(simdjson::simdjson_result<T>& jill,Scene& scene) {
    std::cout << "Loading Scene illumination" << std::endl;

    LightProperties lp{};
    DirectionalLight dl{};

    std::cout << "Setting up Directional Light" << std::endl;
    simdjson::simdjson_result jdl = jill["DirectionalLight"].get_object();

    // Direction
    simdjson::simdjson_result jdl_pos = jdl["Position"].get_array();

    parseVec3(jdl_pos,dl.direction);
    std::cout << "Setting up Directional Light: position" << std::endl;

    // Properties
    std::cout << "Setting up Directional Light: properties" << std::endl;

    auto properties = jdl["Properties"].get_object();
    parseIlluminationProperties(properties,dl.properties);

    scene.illumination.directionalLight = dl;

    // PointLights

}

void buildRenderableObjectFromJSON(simdjson::simdjson_result<simdjson::ondemand::object>& jobj,RenderableObject& obj) {
    std::string_view shape = jobj["Shape"].get_string().value();
    if(shape == "Cube") {
        glm::vec3 origin{};
        auto jorigin = jobj["Position"].get_array();
        parseVec3(jorigin,origin);
        float side = jobj["Side"].get_double().value();
        glm::vec4 colour{};
        auto jcolor = jobj["Colour"].get_array();
        parseVec4(jcolor,colour);
        auto cubeCoords = glTests::createCubeWithNormal(origin,side,{colour,colour,colour,colour,colour,colour,colour,colour});

        obj.vertexHandler = new VertexHandler(std::move(cubeCoords.first),std::move(cubeCoords.second));

        auto jShaderList = jobj["Shaders"].get_array();
        auto shaderList = parseShaderList(jShaderList);
        if(shaderList.size() == 2) obj.shaderHandler = new ShaderHandler(std::move(shaderList.at(0)),std::move(shaderList.at(1)));
        if(shaderList.size() == 3) obj.shaderHandler = new ShaderHandler(std::move(shaderList.at(0)),std::move(shaderList.at(1)),std::move(shaderList.at(2)));
        obj.objModelFun = [](float time) {return glm::mat4(1.f);};

        Material material2{};
        material2.specular = {0.5f, 0.5f, 0.5f};
        material2.shininess = 32.0f;
        obj.enableNormalAttribute();
        obj.setTextureMaterial(material2);
        obj.shaderHandler->setScalarUniform("shadowMap",31);
        obj.shaderHandler->setScalarUniform("depthMap",30);
    }
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
    for(auto obj: renderables) {
        auto _obj = obj.get_object();
        RenderableObject robj{};
        buildRenderableObjectFromJSON(_obj,robj);
        scene.objects.push_back(robj);
    }
    auto illumination = sceneDocument["Illumination"].get_object();
    setupIlluminationFromJSON(illumination,scene);
}

