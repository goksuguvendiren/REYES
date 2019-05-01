//
// Created by Göksu Güvendiren on 2019-04-30.
//

#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm_ostream.hpp"

#include "Mesh.hpp"

namespace rys
{
    class Cone
    {
    public:
        Cone(float h, float r, float t, const glm::mat4& transf) : radius(r), height(h), tmax(t), model_transf(transf), center({0, 0, 0, 1.f})
        {}

        rys::Mesh dice() const
        {
            std::vector<std::vector<Grid>> samples;

            float res = 1 / 360.f;
            for (float v = 0; v <= 1; v += res)
            {
                std::vector<Grid> line;
                for (float u = 0; u <= 1; u += res)
                {
                    auto theta = u * tmax;

                    auto x = radius * (1 - v) * std::cos(glm::radians(theta));
                    auto y = radius * (1 - v) * std::sin(glm::radians(theta));
                    auto z = v * height;

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
        float height;
        float tmax;

        glm::vec4 center;

        glm::vec4 calculate_normal(const glm::vec4& point) const
        {
            auto projected = glm::vec3{point.x - center.x, point.y - center.y, 0};
            projected = glm::normalize(projected);

            auto normal = glm::vec3{projected.x * (height / radius), projected.y * (height / radius), (radius / height)};

            return glm::normalize(glm::vec4(normal, 0.0f));
        }
    };
}
