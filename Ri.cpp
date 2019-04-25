#include <memory>
#include <string>
#include <iostream>
#include "Ri.h"
#include "Reyes.hpp"
#include "Sphere.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
//#include "cimg.h"

namespace rys
{
    std::unique_ptr<rys::reyes> renderer = nullptr;
} // rys

RtToken RI_PERSPECTIVE = RtToken("perspective");
RtToken RI_ORTHOGRAPHIC = RtToken("orthographic");

void RiBegin(RtToken n)
{
    std::string name = "goksus_reyes";
    if (n)
    {
        name = std::string(n);
    }
    rys::renderer = std::make_unique<rys::reyes>(name);
}

void RiEnd()
{
    // does nothing
}

void RiDisplay(char *n, RtToken t, RtToken m, ...)
{
    std::string name = std::string(n);
    std::string ty = std::string(t);
    std::string md = std::string(m);

    rys::Display_Type type;
    if (ty == "" || ty == "file") type = rys::Display_Type::Image;
    else if (ty == "framebuffer") type = rys::Display_Type::Framebuffer;
    else throw std::runtime_error("Unsupported display type!");

    rys::Display_Mode mode;
    if (md == "rgb") mode = rys::Display_Mode::RGB;
    else if (md == "rgba") mode = rys::Display_Mode::RGBA;
    else if (md == "rgbaz") mode = rys::Display_Mode::RGBAZ;
    else if (md == "N") mode = rys::Display_Mode::N;

    rys::renderer->setup_display(type, name, mode);
}

void RiProjection(RtToken n, ...)
{
    rys::Projection_Model model;
    if (strcmp(n, RI_PERSPECTIVE) == 0) model = rys::Projection_Model::Perspective;
    else if (strcmp(n, RI_ORTHOGRAPHIC) == 0) model = rys::Projection_Model::Orthographic;
    else throw std::runtime_error("Incorrect projection model !");

//    std::string name = std::string(n);

//    if (name == "perspective") model = rys::Projection_Model::Perspective;
//    else if (name == "orthographic") model = rys::Projection_Model::Orthographic;
//    else throw std::runtime_error("Incorrect projection model !");

    bool read_fov = false;
    double fov = 45.f;

    // while (*n != '\0')
    // {
    //     std::cerr << n << '\n';
    //     if (*n == 'f')
    //     {
    //         std::string fov_str = va_arg(args, std::string);
    //         assert(fov_str == "fov");
    //         read_fov = true;
    //         continue;
    //         // int i = va_arg(n, int);
    //         // std::cout << i << '\n';
    //     }
    //     if (read_fov)
    //     {
    //         fov = va_arg(args, double);
    //         std::cerr << "fov is : " << fov << '\n';
    //         break;
    //     }
    //     ++n;
    // }

    rys::renderer->setup_projection(model, fov);
}

void RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz)
{
//    glm::mat4 transform = glm::mat4(1.0f);
//
//    transform[0][3] = dx;
//    transform[1][3] = dy;
//    transform[2][3] = dz;

//    std::cerr << transform[0][0] << " " << transform[0][1] << " " << transform[0][2] << " " << transform[0][3] << '\n';
//    std::cerr << transform[1][0] << " " << transform[1][1] << " " << transform[1][2] << " " << transform[1][3] << '\n';
//    std::cerr << transform[2][0] << " " << transform[2][1] << " " << transform[2][2] << " " << transform[2][3] << '\n';
//    std::cerr << transform[3][0] << " " << transform[3][1] << " " << transform[3][2] << " " << transform[3][3] << '\n';

    auto glm_tr = glm::translate(glm::vec3{dx, dy, dz});
//    assert(glm::transpose(transform) == glm_tr);

    rys::renderer->add_transform(glm_tr);
}

void RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
    glm::mat4 transform = glm::mat4(1.0f);

//    assert(dy == 0 && dz == 0);

    double cos_th = std::cos(angle);
    double sin_th = std::sin(angle);

    glm::mat4 around_x = glm::mat4(1.0f);
    around_x[1][1] = cos_th;
    around_x[1][2] = -sin_th;
    around_x[2][1] = sin_th;
    around_x[2][2] = cos_th;

//    glm::mat4 around_y = glm::mat4(1.0f);
//    around_y[0][0] = cos_th;
//    around_y[0][2] = sin_th;
//    around_y[2][0] = -sin_th;
//    around_y[2][2] = cos_th;
//
//    glm::mat4 around_z = glm::mat4(1.0f);
//    around_z[0][0] = cos_th;
//    around_z[0][1] = -sin_th;
//    around_z[1][0] = sin_th;
//    around_z[1][1] = cos_th;

    auto glm_rt = glm::rotate(angle, glm::vec3{dx, dy, dz});
//    assert((around_x * around_y * around_z) == glm_rt);
//    assert(glm::transpose(around_x * around_y * around_z) == glm_rt);

//    std::cout << glm::transpose(around_x) << '\n';
//    std::cout << glm_rt << '\n';

    rys::renderer->add_transform(glm_rt);
//    rys::renderer->add_transform(glm::transpose(around_x));
}

// void RiScale(RtFloat sx, RtFloat sy, RtFloat sz)
// {

// }

void RiFrameBegin(RtInt frame)
{
    rys::renderer->set_cur_frame_id(frame);
    auto& frame_buffer = rys::renderer->get_frame_buffer();
    std::fill(frame_buffer.begin(), frame_buffer.end(), glm::vec3{0, 0, 0});    // clear the frame buffer
}

void RiFrameEnd()
{
}

void RiWorldBegin()
{
    // freeze all options about renderer!
    // move current_matrix to world_to_camera
    // reinitialize current_matrix to identity
    // after this, the current_matrix is interpreted as obj_to_world matrix
    // now, can accept the geometry primitives.
    rys::renderer->world_begin();
}

void RiWorldEnd()
{
    // Does not normally return until the rendering program has completed
    // drawing the image. If the image is to be saved in a file, this is done automatically by
    // this function.
    // All lights and retained objects defined inside the RiWorldBegin-RiWorldEnd world
    // block are removed and their storage reclaimed when RiWorldEnd is called (thus invalidating
    // their handles)
    //TODO : Display / Write the frame!
    rys::renderer->save_frame();
}

void RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...)
{
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, 5.0));
    model = glm::rotate(model, -175.0f, glm::vec3(0, 1, 0));
    model = glm::rotate(model, 110.0f,  glm::vec3(1, 0, 0));
    rys::Sphere sphere(radius, zmin, zmax, tmax, model);
    rys::renderer->render(sphere);
}

void RiColor(RtColor color)
{
    rys::renderer->set_color({color[0], color[1], color[2]});
}

void RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
    rys::renderer->set_format(xres, yres, aspect);
}

void RiFrameAspectRatio(RtFloat ratio)
{
    rys::renderer->set_viewport_aspect(ratio);
}

void RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
    rys::renderer->set_pixel_samples(xsamples, ysamples);
}

void RiShutter(RtFloat min, RtFloat max)
{
    if (min != max)
        throw std::runtime_error("Motion blur not implemented yet");
    // Sets the shutter time, it starts at t = min, and ends at t = max.
    // If min == max, then no motion blur is done.
}

void RiTransformBegin(void)
{
    rys::renderer->push_current_matrix();
}

void RiTransformEnd(void)
{
    rys::renderer->pop_current_matrix();
}