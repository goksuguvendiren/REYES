//
// Created by goksu on 4/30/19.
//

#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "Mesh.hpp"

namespace rys
{
    class Cylinder
    {
    public:
        Cylinder(float r, float zmn, float zmx, float t, const glm::mat4& transf) : radius(r), zmin(zmn), zmax(zmx), tmax(t), model_transf(transf)
        {}

        rys::Mesh dice() const
        {
            std::vector<std::vector<Grid>> samples;

            float res = 10 / 360.f;
            for (float v = 0; v <= 1; v += res)
            {
                std::vector<Grid> line;
                for (float u = 0; u <= 1; u += res)
                {
                    auto theta = u * tmax;

                    auto x = radius * std::cos(glm::radians(theta));
                    auto y = radius * std::sin(glm::radians(theta));
                    auto z = v * (zmax - zmin);

                    glm::vec4 local_coord = glm::vec4(x, y, z, 1.0f);
                    glm::vec4 local_normal = calculate_normal(local_coord);

                    glm::vec4 world_coord = model_transf * local_coord;
                    glm::vec4 normal = glm::transpose(glm::inverse(model_transf)) * local_normal;
                    normal.w = 0;

                    Grid g;
                    g.position = world_coord;
                    g.normal = glm::normalize(normal);
                    g.uv = glm::vec2(u, v);

                    line.emplace_back(g);
                }
                samples.push_back(line);
            }

            return Mesh(samples);
        }

    private:
        glm::mat4 model_transf;

        float radius;
        float zmin;
        float zmax;
        float tmax;

        glm::vec4 calculate_normal(const glm::vec4& point) const
        {
            return glm::normalize(glm::vec4(point.x, point.y, 0, 0));
        }
    };
}
