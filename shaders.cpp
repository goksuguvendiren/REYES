//
// Created by goksu on 4/27/19.
//

#include "shaders.hpp"
#include <glm/glm.hpp>
#include <cassert>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

//extern int CHECK_SIZE_X;
//extern int CHECK_SIZE_Y;

//glm::vec4 BUMPY(const glm::vec4& position)
//{
//    auto floor = [](float p) { return int(p / 1.f); };
//    for (int i = floor(position.x); i <= floor(position.x) + 1; ++i)
//    {
//        for (int j = floor(position.y); j <= floor(position.y) + 1; ++j)
//        {
//            for (int k = floor(position.z); k <= floor(position.z) + 1; ++k)
//            {
//
//            }
//        }
//    }
//}

void NONE(surface_shader_payload& payload)
{
    payload.color = payload.color;
}

void CHECKERBOARD(surface_shader_payload& payload)
{
    bool u_white = (int(payload.uv.x * 10) % 2) == 1;
    bool v_white = (int(payload.uv.y * 10) % 2) == 1;

    auto val = u_white ^ v_white;
    payload.color = {val, val, val, 1.0f};
}

void UV(surface_shader_payload& payload)
{
    payload.color = {0, payload.uv.y, 0, 1.0f};
}


static glm::vec4 reflect(const glm::vec3& vec, const glm::vec3& axis)
{
//    assert(glm::length(vec) == 1 && glm::length(axis) == 1);

    auto costheta = glm::dot(glm::vec3(vec), glm::vec3(axis));
    return glm::vec4(2 * costheta * axis - vec, 0.f);
}

void PHONG(surface_shader_payload& payload)
{
    float ka = 0.1;
    float kd = 0.6;
    float ks = 0.3;

    glm::vec3 light_position{2, -2, -10};
    glm::vec3 light_intensity{2, 2, 2};
    glm::vec3 eye_pos{0, 0, 0};

    glm::vec3 color = payload.color; // TODO : if texture, sample this color from the texture.
    glm::vec3 point = payload.position;
    glm::vec3 normal = payload.normal;

    glm::vec3 light_dir = glm::normalize(light_position - point);
    glm::vec3 view_dir  = glm::normalize(eye_pos - point);

    glm::vec3 R = reflect(light_dir, normal);

    auto cosalpha = glm::dot(normal, light_dir);

    float p = 10;

    auto ambient = ka * color;
    auto diffuse = kd * color * light_intensity * std::max(0.f, cosalpha);
    auto specular = ks * light_intensity * std::pow(std::max(0.f, glm::dot(R, view_dir)), p);

    // output color:
    payload.color = glm::vec4(ambient + diffuse + specular, 1.0f); //payload.color / glm::pi<float>() * light_intensity * std::max(0.f, cosalpha);
}

void NORMAL(surface_shader_payload& payload)
{
    payload.color = glm::vec4((payload.normal + glm::vec3{1.f, 1.f, 1.f}) / 2.0f, 1.0f);
}

void EARTHSHADER(surface_shader_payload& payload)
{
    auto texture = payload.texture;
    payload.color = glm::vec4{texture->query(payload.uv.x, payload.uv.y) / 255.f, 1.0f};
}