#include "Reyes.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

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

    pixel_xsamples = 1;
    pixel_ysamples = 1;
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

    real_width = xres;
    real_height = yres;
    aspect_ratio = pix_asp_ratio;
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
    cv::resize(image, image, cv::Size{(int)real_width, (int)real_height});

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


glm::vec2i rys::reyes::get_ss_coords(const glm::vec4& point)
{
    auto unit_cube = proj_matrix * view_matrix * point;
    unit_cube /= unit_cube.w;

    unit_cube = viewport_matrix * unit_cube;

    glm::vec2 ss_coords(unit_cube);
    ss_coords.x = std::min(ss_coords.x, (float)width - 1);
    ss_coords.x = std::max(ss_coords.x, 0.f);
    ss_coords.y = std::min(ss_coords.y, (float)height - 1);
    ss_coords.y = std::max(ss_coords.y, 0.f);

    return {(int)ss_coords.x, (int)ss_coords.y};
}

std::pair<glm::vec2i, glm::vec2i> find_bounding_box(const rys::polygon& mpoly)
{
    glm::vec2i bb_min;
    glm::vec2i bb_max;

    bb_min.x = std::min(std::min(std::min(mpoly.current.x, mpoly.right.x), mpoly.below.x), mpoly.cross.x);
    bb_min.y = std::min(std::min(std::min(mpoly.current.y, mpoly.right.y), mpoly.below.y), mpoly.cross.y);

    bb_max.x = std::max(std::max(std::max(mpoly.current.x, mpoly.right.x), mpoly.below.x), mpoly.cross.x);
    bb_max.y = std::max(std::max(std::max(mpoly.current.y, mpoly.right.y), mpoly.below.y), mpoly.cross.y);

    return {bb_min, bb_max};
}
//
//std::vector<int> rys::reyes::find_intersecting_samples(const std::pair<glm::vec2i, glm::vec2i>& bb, const rys::polygon& mpoly)
//{
//    auto bb_min = bb.first;
//    auto bb_max = bb.second;
//    auto top_left = glm::vec2i{bb_min.x, bb_max.y};
//    auto bot_right = glm::vec2i{bb_max.x, bb_min.y};
//
//    auto tri1 = rys::triangle{mpoly.current, mpoly.right, mpoly.below};
//    auto tri2 = rys::triangle{mpoly.right, mpoly.below, mpoly.cross};
//
//    for (int i = top_left.y * pixel_ysamples; i <= bot_right.y * pixel_ysamples; ++i)
//    {
//        for (int j = top_left.x * pixel_xsamples; j <= bot_right.x * pixel_xsamples; ++j)
//        {
////            if (tri1.intersects(sample_buffer[i][j]) || tri2.intersects(sample_buffer[i][j]))
////            {
////                sample_buffer[i][j]->set_color(get_color());
////            }
//        }
//    }
//}

void rys::reyes::render(const rys::Sphere &sphere)
{
    auto mesh = sphere.dice();

    auto samples = mesh.get_samples();
    for (int i = 0; i < samples.size(); ++i)
    {
        for (int j = 0; j < samples[0].size(); ++j)
        {
            auto current = get_ss_coords(samples[i][j]);
            auto right   = get_ss_coords(samples[i][(j+1) % samples[0].size()]);
            auto below   = get_ss_coords(samples[(i+1) % samples.size()][j]);
            auto cross   = get_ss_coords(samples[(i+1) % samples.size()][(j+1) % samples[0].size()]);

            auto mpoly = rys::polygon{current, right, below, cross};

            auto bounding_box = find_bounding_box(mpoly);
            auto bb_min = bounding_box.first;
            auto bb_max = bounding_box.second;
            auto top_left = glm::vec2i{bb_min.x, bb_min.y};
            auto bot_right = glm::vec2i{bb_max.x, bb_max.y};

//            for (int i = top_left.y; i <= bot_right.y; ++i)
//            {
//                for (int j = top_left.x; j <= bot_right.x; ++j)
//                {
//                    paint_pixel(j, i, get_color());
//                }
//            }

//            auto intersecting_samples = find_intersecting_samples(bounding_box, mpoly);
            // then, set the current color to the intersecting samples.

            paint_pixel(current.x, current.y, get_color());
        }
    }
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

    proj_matrix = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);
}

void rys::reyes::initialize_buffers()
{
    auto& frame_buffer = get_frame_buffer();
    std::fill(frame_buffer.begin(), frame_buffer.end(), glm::vec3{0, 0, 0});    // clear the frame buffer

//    auto& samples_buffer = get_sample_buffer();

//    std::random_device rd;
//    std::mt19937 mt(rd());
//    std::uniform_real_distribution<double> dist(0.0, 1.0);

//    for (int i = 0; i < height * pixel_ysamples; ++i)
//    {
//        for (int j = 0; j < width * pixel_xsamples; ++j)
//        {
//            auto x = dist(mt);
//            auto y = dist(mt);
//
//            auto ind = i * width * pixel_xsamples + j;
//            samples_buffer[ind] = glm::vec2{x, y};
//        }
//    }
}

void rys::reyes::set_pixel_samples(int xsamples, int ysamples)
{
    pixel_xsamples = xsamples;
    pixel_ysamples = ysamples;

    width  *= pixel_xsamples;
    height *= pixel_ysamples;
}

void rys::reyes::initialize_viewport()
{
    viewport_aspect_ratio = (width * aspect_ratio) / (float)height;

    viewport_matrix = glm::mat4(1.0f);
    viewport_matrix[0][0] = width / 2.f;
    viewport_matrix[0][3] = width / 2.f;
    viewport_matrix[1][1] = height / 2.f;
    viewport_matrix[1][3] = height / 2.f;

    viewport_matrix = glm::transpose(viewport_matrix);
}

