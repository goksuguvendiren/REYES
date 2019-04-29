//
// Created by goksu on 4/27/19.
//

#include "shaders.hpp"
#include <glm/glm.hpp>
#include <cassert>
#include <glm/ext/scalar_constants.hpp>

glm::vec4 BUMPY(const glm::vec4& position)
{

}

void NONE(surface_shader_payload& payload)
{
    payload.color = payload.color;
}

void CHECKERBOARD(surface_shader_payload& payload)
{

}

static glm::vec3 reflect(const glm::vec3& vec, const glm::vec3& axis)
{
//    assert(glm::length(vec) == 1 && glm::length(axis) == 1);

    auto angle = glm::dot(vec, axis);

    return glm::normalize(axis - vec);
//    return glm::normalize(2 * angle * axis - vec);
}

//glm::vec4 eye_pos{0, 0, 0, 1.0f};
//auto view = eye_pos - point;
//
//auto R = reflect(glm::normalize(light_dir), normal);
//auto specular = glm::dot(R, glm::normalize(view));
//specular = std::pow(specular, 10);

void PHONG(surface_shader_payload& payload)
{
    glm::vec4 light_position{-2, -2, -10, 1.f};
    glm::vec4 light_intensity{5, 5, 5, 1};

    auto point = payload.position;
    auto normal = payload.normal;
    auto light_dir = light_position - point;

    auto cosalpha = glm::dot(glm::vec3(normal), glm::vec3(glm::normalize(light_dir)));

    // output color:
    payload.color = payload.color / glm::pi<float>() * light_intensity * std::max(0.f, cosalpha);
}