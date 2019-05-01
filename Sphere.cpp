//
// Created by Göksu Güvendiren on 2019-04-24.
//

#include <vector>
#include "Sphere.hpp"
#include "Mesh.hpp"

rys::Mesh rys::Sphere::dice() const
{
    //TODO check asin result
    auto phi_min = zmin > -radius ? std::asin(zmin / radius) : -90.0f;
    auto phi_max = zmax < radius  ? std::asin(zmax / radius) :  90.0f;

    std::vector<std::vector<Grid>> samples;

    float res = 1 / 360.f;
    for (float u = 0; u <= 1; u += res)
    {
        std::vector<Grid> line;
        for (float v = 0; v <= 1; v += res * 2)
        {
            Grid g;
            auto phi = phi_min + v * (phi_max - phi_min);
            auto theta = u * tmax;
//
            auto x = radius * std::cos(glm::radians(phi)) * std::cos(glm::radians(theta));
            auto y = radius * std::sin(glm::radians(phi));
            auto z = radius * std::cos(glm::radians(phi)) * std::sin(glm::radians(theta));

            glm::vec4 local_coord = glm::vec4(x, y, z, 1.0f);
            glm::vec4 normal = (local_coord - glm::vec4{0, 0, 0, 1.0f});

            glm::vec4 world_coord = model_transf * local_coord;
            g.position = world_coord;
            g.normal = glm::normalize(world_coord - cent);
            g.uv = glm::vec2(u, v);
            line.emplace_back(g);
        }
        samples.push_back(line);
    }

    return Mesh(samples);
}