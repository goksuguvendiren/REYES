//
// Created by Göksu Güvendiren on 2019-04-24.
//

#pragma once

#include <ostream>
#include <glm/glm.hpp>

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
    os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat4& vec)
{
    os << vec[0][0] << ", " << vec[0][1] << ", " << vec[0][2] << ", " << vec[0][3] << '\n';
    os << vec[1][0] << ", " << vec[1][1] << ", " << vec[1][2] << ", " << vec[1][3] << '\n';
    os << vec[2][0] << ", " << vec[2][1] << ", " << vec[2][2] << ", " << vec[2][3] << '\n';
    os << vec[3][0] << ", " << vec[3][1] << ", " << vec[3][2] << ", " << vec[3][3];

    return os;
}
