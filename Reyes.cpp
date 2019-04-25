#include "Reyes.hpp"
#include <opencv2/opencv.hpp>
//#include "cimg.h"
#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

//namespace cimg = cimg_library;

rys::reyes::reyes(const std::string& n) : near(0.1f), far(100.f)
{
    world_began = false;
    name = n;
    fov = 45.f;

    disp_mode = Display_Mode::RGB;
    disp_type = Display_Type::Image;
    proj_model = Projection_Model::Perspective;

    proj_matrix = glm::mat4(1.0f);
    current_matrix = glm::mat4(1.0f);
    view_matrix = glm::lookAt(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0});

    current_color = glm::vec3(1.0f);
}

void rys::reyes::world_begin()
{
    world_began = true;
    view_matrix = view_matrix * current_matrix;
    current_matrix = glm::mat4(1.0f);

    //after this, the current_matrix represents the obj_to_world transformation.
}

void rys::reyes::add_transform(const glm::mat4& transform)
{
    current_matrix = transform * current_matrix;
}

void rys::reyes::set_format(int xres, int yres, float pix_asp_ratio)
{
    width = xres;
    height = yres;
    aspect_ratio = pix_asp_ratio;
    viewport_aspect_ratio = (width * aspect_ratio) / (float)height;

    viewport_matrix = glm::mat4(1.0f);
    viewport_matrix[0][0] = width / 2.f;
    viewport_matrix[0][3] = width / 2.f;
    viewport_matrix[1][1] = height / 2.f;
    viewport_matrix[1][3] = height / 2.f;

//    viewport_matrix = glm::transpose(viewport_matrix);
}

// either show the framebuffer or save it to a file.
void rys::reyes::setup_display(Display_Type type, const std::string& n, Display_Mode mode)
{
    name = n;
    disp_type = type;
    disp_mode = mode;
}

std::vector<glm::vec3>& rys::reyes::get_frame_buffer()
{
    if (frame_buffer.empty())
    {
        frame_buffer.resize(width * height);
    }
    return frame_buffer;
}

void rys::reyes::save_frame()
{
    std::vector<float> float_image;
    for (int i = 0; i < height * width; ++i)
    {
        float_image.push_back(frame_buffer[i].x);
        float_image.push_back(frame_buffer[i].y);
        float_image.push_back(frame_buffer[i].z);
    }

    // rows, cols, type, data
    cv::Mat image(height, width, CV_32FC3, float_image.data());

//    cimg::CImg<float> image(width, height, 1, 3, 255.f);
//    cimg::CImg<unsigned char> img_normalized = image.get_normalize(0,255);


//    std::cerr << img_normalized.width() << '\n';
//    std::cerr << img_normalized.height() << '\n';

    cv::imshow("debug", image);
    cv::waitKey(0);

    switch(disp_type)
    {
        case Display_Type::Image:
            std::cerr << "Saving the image to : " << name << '\n';
            cv::imwrite(name, image);
//            img_normalized.save(name.c_str());
            break;
        case Display_Type::Framebuffer:
//            cimg::CImgDisplay main_disp(img_normalized, name.c_str());
            break;
    }

    if (disp_mode != Display_Mode::RGB)
    {
        throw std::runtime_error("Display mode other than RGB isn't implemented!");
    }

    // switch(mode)
    // {
    //     case Display_Mode::N:
    //         break;
    //     case Display_Mode::RGB:
    //         break;
    //     case Display_Mode::RGBA:
    //         break;
    //     case Display_Mode::RGBAZ:
    //         break;
    // }
}

void rys::reyes::paint_pixel(int x, int y, const glm::vec3 &color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Invalid pixel indices!");

    auto ind = y * width + x;
    frame_buffer[ind] = color;
}

void rys::reyes::render(const rys::Sphere &sphere)
{
//    auto mesh = sphere.dice();

    glm::vec4 asd = proj_matrix * view_matrix * glm::vec4(sphere.get_center(), 1.0f);
    asd /= asd.w;

    asd = glm::transpose(viewport_matrix) * asd;

    glm::vec2 ss_coords(asd);

    paint_pixel(ss_coords.x, ss_coords.y, get_color());
}

void rys::reyes::push_current_matrix()
{
    transform_stack.push(current_matrix);
    current_matrix = glm::mat4(1.0f);
}

void rys::reyes::pop_current_matrix()
{
    auto top = transform_stack.top();
    transform_stack.pop();

    current_matrix = top;
}

static double radians(float degrees)
{
    return degrees * (rys::pi / 180.f);
}

void rys::reyes::setup_projection(rys::Projection_Model model, float f)
{
    //TODO : Implement ortho projection!
    proj_model = model;
    fov = f;

//    proj_matrix = glm::mat4(1.0f);
//
//    auto y_scale = 1.f / std::tan(radians(fov) / 2.f);
//    auto x_scale = y_scale / viewport_aspect_ratio;
//
//    proj_matrix[0][0] = x_scale;
//    proj_matrix[1][1] = y_scale;
//    proj_matrix[2][2] = (near + far) / (near - far);
//    proj_matrix[2][3] = -1;
//    proj_matrix[3][3] = 2 * near * far / (near - far);
//
//    proj_matrix = glm::transpose(proj_matrix);
    double f_o_v = radians(f);
//    proj_matrix = glm::perspectiveFov<double>(f, width, height, near, far);
//    proj_matrix = glm::perspective(45.f, viewport_aspect_ratio, near, far);
    proj_matrix = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);
}

