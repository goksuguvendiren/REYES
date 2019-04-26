//
// Created by Göksu Güvendiren on 2019-04-24.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace rys
{
    class Mesh
    {
        std::vector<std::vector<glm::vec4>> samples;
    public:
        explicit Mesh(std::vector<std::vector<glm::vec4>> s) : samples(std::move(s)) {}

        std::vector<std::vector<glm::vec4>> get_samples() { return samples; }
    };
}