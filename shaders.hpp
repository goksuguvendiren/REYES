//
// Created by goksu on 4/27/19.
//

#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct surface_shader_payload
{
    glm::vec2 uv;
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 normal;
};

// displacement_shader
glm::vec4 BUMPY(const glm::vec4& position);

void NONE(surface_shader_payload& payload);

void CHECKERBOARD(surface_shader_payload& payload);

void PHONG(surface_shader_payload& payload);