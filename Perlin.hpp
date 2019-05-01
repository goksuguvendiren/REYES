//
// Created by Göksu Güvendiren on 2019-05-01.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace rys
{

    enum class Noise_Appeareance
    {
        Patch,
        Vein
    };

    class Perlin
    {
        float scalingFactor;
        Noise_Appeareance app;

        std::vector<int> P;

        glm::vec3 e(int i, int j, int k) const;

        int phi(int i, const std::vector<int> &P) const;

        float weight(float t) const;

        float omega(glm::vec3 uvw, int i, int j, int k) const;

    public:
        Perlin(float sf, Noise_Appeareance app = Noise_Appeareance::Patch);

        float query(const glm::vec3 &point) const;

        auto Type() const
        { return app; }
    };
}
