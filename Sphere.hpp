//
// Created by goksu on 4/20/19.
//

#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm_ostream.hpp"

#include "Mesh.hpp"

namespace rys
{
    class Sphere
    {
    public:
        Sphere(float rad, float zmn, float zmx, float tmx, const glm::mat4& transf) : radius(rad), model_transf(transf),
        cent({0, 0, 0, 1.0f}), zmin(zmn), zmax(zmx), tmax(tmx)
        {
            cent = model_transf * cent;
        }

        glm::vec3 get_center() const { return cent; }

        rys::Mesh dice() const;
    private:
        glm::mat4 model_transf;
        glm::vec4 cent;
        float zmin;
        float zmax;
        float tmax;

        float radius;
    };
}
