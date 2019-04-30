//
// Created by goksu on 4/27/19.
//

#include "shaders.hpp"
#include <glm/glm.hpp>
#include <cassert>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

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
    bool u_white = (int(payload.uv.x * 20) % 2) == 1;
    bool v_white = (int(payload.uv.y * 15) % 2) == 1;

//    std::cout << payload.uv.x <<'\n';

    if (v_white)
    {
        if (u_white)
        {
            payload.color = glm::vec4{1, 1, 1, 1.f};
        }
        else payload.color = glm::vec4(0, 0, 0, 1.f);
    }
    else
    {
        if (!u_white)
        {
            payload.color = glm::vec4{1, 1, 1, 1.f};
        }
        else payload.color = glm::vec4(0, 0, 0, 1.f);
    }
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

    glm::vec4 light_position{2, -2, -10, 1.f};
    glm::vec4 light_intensity{2, 2, 2, 1};
    glm::vec4 eye_pos{0, 0, 0, 1.0f};

    glm::vec4 color = payload.color; // TODO : if texture, sample this color from the texture.
    glm::vec4 point = payload.position;
    glm::vec4 normal = payload.normal;

    glm::vec4 light_dir = glm::normalize(light_position - point);
    glm::vec4 view_dir  = glm::normalize(eye_pos - point);

    glm::vec4 R = reflect(light_dir, normal);

    auto cosalpha = glm::dot(glm::vec3(normal), glm::vec3(light_dir));

    float p = 10;

    auto ambient = ka * color;
    auto diffuse = kd * color * light_intensity * std::max(0.f, cosalpha);
    auto specular = ks * light_intensity * std::pow(std::max(0.f, glm::dot(R, view_dir)), p);

    // output color:
    payload.color = ambient + diffuse; //payload.color / glm::pi<float>() * light_intensity * std::max(0.f, cosalpha);
}

void EARTHSHADER(surface_shader_payload& payload)
{
    auto texture = payload.texture;
    payload.color = glm::vec4{texture->query(payload.uv.x, payload.uv.y) / 255.f, 1.0f};
}