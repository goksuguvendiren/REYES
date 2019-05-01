//
// Created by goksu on 5/1/19.
//

#pragma once


#include <glm/ext/matrix_float4x4.hpp>
#include "Mesh.hpp"

namespace rys
{
    class Torus
    {
    public:
        Torus(float major_rad, float minor_rad, float phimn, float phimx, float t, const glm::mat4& transf) :
            major_radius(major_rad), minor_radius(minor_rad), phimin(phimn), phimax(phimx), tmax(t), model_transf(transf)
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
                    auto phi   = phimin + v * (phimax - phimin);
                    auto r = minor_radius * std::cos(glm::radians(phi));
                    auto z = minor_radius * std::sin(glm::radians(phi));
                    auto x = (major_radius + r) * std::cos(glm::radians(theta));
                    auto y = (major_radius + r) * std::sin(glm::radians(theta));

                    glm::vec4 local_coord = glm::vec4(x, y, z, 1.0f);
                    glm::vec4 local_normal = calculate_normal(local_coord, glm::radians(theta), glm::radians(phi));

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

        float major_radius;
        float minor_radius;
        float phimin;
        float phimax;
        float tmax;

        glm::vec4 calculate_normal(const glm::vec4& point, float theta, float phi) const
        {
            auto tx = -sin(theta);
            auto ty = cos(theta);
            auto tz = 0;

            auto sx = cos(theta)*(-sin(phi));
            auto sy = sin(theta)*(-sin(phi));
            auto sz = cos(phi);

            auto nx = ty*sz - tz*sy;
            auto ny = tz*sx - tx*sz;
            auto nz = tx*sy - ty*sx;

            return glm::normalize(glm::vec4(nx, ny, nz, 0.0f));
        }
    };
}
