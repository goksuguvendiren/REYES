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

    std::vector<std::vector<glm::vec4>> samples;

    float res = 36 / 360.f;
    for (float u = 0; u <= 1; u += res)
    {
        std::vector<glm::vec4> line;
        for (float v = 0; v <= 1; v += res * 2)
        {
//            std::cerr << u << ", " << v << ": ";
            auto phi = phi_min + v * (phi_max - phi_min);
            auto theta = u * tmax;
//
//            std::cout << phi << ", ";
//            std::cout << theta << '\n';

            auto x = radius * std::cos(glm::radians(phi)) * std::cos(glm::radians(theta));
            auto y = radius * std::sin(glm::radians(phi));
            auto z = radius * std::cos(glm::radians(phi)) * std::sin(glm::radians(theta));

//            std::cerr << x << ", " << y << ", " << z << "\n";
            glm::vec4 local_coord = glm::vec4(x, y, z, 1.0f);
            auto world_coord = model_transf * local_coord;
            line.emplace_back(world_coord);
        }
        samples.push_back(line);
    }

    return Mesh(samples);
}