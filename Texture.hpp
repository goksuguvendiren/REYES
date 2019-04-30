//
// Created by Göksu Güvendiren on 2019-04-30.
//

#pragma once
#include <opencv2/opencv.hpp>
#include <glm/vec3.hpp>


class Texture
{
    cv::Mat image_data;

public:
    explicit Texture(const std::string& filename)
    {
        image_data = cv::imread(filename);
        cv::cvtColor(image_data, image_data, cv::COLOR_BGR2RGB);
    }

    glm::vec3 query(float u, float v)
    {
        int x = int(u * image_data.cols);
        int y = int(v * image_data.rows);

        auto color = image_data.at<cv::Vec3b>(y, x);
        return {color[0], color[1], color[2]};
    }
};

