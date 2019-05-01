//
// Created by goksu on 4/27/19.
//

#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Texture.hpp"

struct surface_shader_payload
{
    glm::vec2 uv;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
    Texture* texture;
};

// displacement_shader
glm::vec3 BUMPY(const glm::vec3& position, const glm::vec3& normal);

void NONE(surface_shader_payload& payload);

//int CHECK_SIZE_X;
//int CHECK_SIZE_Y;

void CHECKERBOARD(surface_shader_payload& payload);
void POS(surface_shader_payload& payload);

void UV(surface_shader_payload& payload);
void NORMAL(surface_shader_payload& payload);
void PHONG(surface_shader_payload& payload);

void EARTHSHADER(surface_shader_payload& payload);
void NOISE(surface_shader_payload& payload);