//
// Created by Göksu Güvendiren on 2019-04-24.
//

#include <vector>
#include "Sphere.hpp"
#include "Mesh.hpp"

rys::Mesh rys::Sphere::dice() const
{
    auto phi_min = zmin > -radius ? std::asin(zmin / radius) : -90.0f;
    auto phi_max = zmax < radius  ? std::asin(zmax / radius) :  90.0f;

    float res = 10 / 360.f;
    for (float u = 0; u < 1; u += res)
    {
        for (float v = 0; v < 1; v += res * 2)
        {
            std::cerr << u << ", " << v << ": ";
            auto phi = phi_min + v * (phi_max - phi_min);
            auto theta = u * tmax;

            auto x = radius * std::cos(glm::radians(theta)) * std::cos(glm::radians(phi));
            auto y = radius * std::sin(glm::radians(theta)) * std::cos(glm::radians(phi));
            auto z = radius * std::sin(glm::radians(theta)) * std::cos(glm::radians(phi));

            std::cerr << x << ", " << y << ", " << z << "\n";
        }
    }

//
//    int res = 10; // resolution_in_angles
//    std::vector<std::vector<std::pair<int, int>>> samples_in_angles;
//
//    for (int phi = 0; phi < 180; phi += res)
//    {
//        std::vector<std::pair<int, int>> samples;
//        for (int theta = 0; theta < 360; theta += res)
//        {
//            samples.emplace_back(phi, theta);
//        }
//        samples_in_angles.push_back(samples);
//    }
//
//    for (int i = 0; i < samples_in_angles.size(); ++i)
//    {
//        for (int j = 0; j < samples_in_angles[0].size(); ++j)
//        {
//            std::cerr << "(" << samples_in_angles[i][j].first << ", " << samples_in_angles[i][j].second << "), ";
//        }
//        std::cerr << '\n';
//    }
}