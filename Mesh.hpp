//
// Created by Göksu Güvendiren on 2019-04-24.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace rys
{
    struct Grid
    {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec4 normal;
        glm::vec4 tangent;
        glm::vec3 ss_coords;
        glm::vec2 uv;
    };

    class Mesh
    {
        std::vector<std::vector<Grid>> grid;
    public:
        explicit Mesh(std::vector<std::vector<Grid>> g) : grid(std::move(g)) {}

        std::vector<std::vector<Grid>>& get_grid() { return grid; }
    };
}