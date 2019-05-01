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
    view_matrix = glm::lookAt(glm::vec3{0, 0, -1}, glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0});

    current_color = glm::vec3(1.0f);

    pixel_xsamples = 1;
    pixel_ysamples = 1;

    displacement_shader = rys::default_displacement_shader;
    surface_shader      = rys::default_surface_shader;

    texture = std::nullopt;
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
    current_matrix = current_matrix * transform;
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

std::vector<rys::sample>& rys::reyes::create_frame_buffer()
{
    if (frame_buffer.empty())
    {
        frame_buffer.resize(width * height);
    }
    return frame_buffer;
}

std::vector<float>& rys::reyes::create_depth_buffer()
{
    if (depth_buffer.empty())
    {
        depth_buffer.resize(width * height);
    }
    return depth_buffer;
}

void rys::reyes::save_frame()
{
    std::vector<float> float_image;
    for (int i = 0; i < height * width; ++i)
    {
        float_image.push_back(frame_buffer[i].color.x);
        float_image.push_back(frame_buffer[i].color.y);
        float_image.push_back(frame_buffer[i].color.z);
    }

    // rows, cols, type, data
    cv::Mat image(height, width, CV_32FC3, float_image.data());
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::resize(image, image, cv::Size{(int)real_width, (int)real_height});

//    cimg::CImg<float> image(width, height, 1, 3, 255.f);
//    cimg::CImg<unsigned char> img_normalized = image.get_normalize(0,255);


//    std::cerr << img_normalized.width() << '\n';
//    std::cerr << img_normalized.height() << '\n';

    cv::imshow("debug", image);
    cv::imwrite("output.png", image * 255);
    cv::waitKey(0);

    return;
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
    frame_buffer[ind].color = color;
}

rys::sample rys::reyes::get_pixel(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Invalid pixel indices!");

    auto ind = y * width + x;
    return frame_buffer[ind];
}

float rys::reyes::get_depth(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Invalid pixel indices!");

    auto ind = y * width + x;
    return depth_buffer[ind];
}

void rys::reyes::set_depth(int x, int y, float n_depth)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Invalid pixel indices!");

    auto ind = y * width + x;
    depth_buffer[ind] = n_depth;
}

glm::vec3 rys::reyes::get_ss_coords(const glm::vec4& point)
{
    auto camera = view_matrix * point;
    auto unit_cube = proj_matrix * camera;
    unit_cube /= unit_cube.w;

    unit_cube = viewport_matrix * unit_cube;

    glm::vec2 ss_coords(unit_cube);
    ss_coords.x = std::min(ss_coords.x, (float)width - 1);
    ss_coords.x = std::max(ss_coords.x, 0.f);
    ss_coords.y = std::min(ss_coords.y, (float)height - 1);
    ss_coords.y = std::max(ss_coords.y, 0.f);

    return {(int)ss_coords.x, (int)ss_coords.y, unit_cube.z};
}

std::pair<glm::vec2i, glm::vec2i> rys::reyes::find_bounding_box(const rys::polygon& mpoly)
{
    glm::vec2i bb_min;
    glm::vec2i bb_max;

    auto current = get_ss_coords(mpoly.current.position);
    auto right = get_ss_coords(mpoly.right.position);
    auto below = get_ss_coords(mpoly.below.position);
    auto cross = get_ss_coords(mpoly.cross.position);

    bb_min.x = std::min(std::min(std::min(current.x, right.x), below.x), cross.x);
    bb_min.y = std::min(std::min(std::min(current.y, right.y), below.y), cross.y);

    bb_max.x = std::max(std::max(std::max(current.x, right.x), below.x), cross.x);
    bb_max.y = std::max(std::max(std::max(current.y, right.y), below.y), cross.y);

    return {bb_min, bb_max};
}

void rys::reyes::paint_intersecting_samples(const std::pair<glm::vec2i, glm::vec2i>& bb, const rys::polygon& mpoly)
{
    auto bb_min = bb.first;
    auto bb_max = bb.second;
    auto top_left = glm::vec2i{bb_min.x, bb_min.y};
    auto bot_right = glm::vec2i{bb_max.x, bb_max.y};

    float average_depth = mpoly.get_average_depth();

    auto current = get_ss_coords(mpoly.current.position);
    auto right = get_ss_coords(mpoly.right.position);
    auto below = get_ss_coords(mpoly.below.position);
    auto cross = get_ss_coords(mpoly.cross.position);

    auto tri1 = rys::triangle{current, right, below};
    auto tri2 = rys::triangle{right, below, cross};

    for (int i = top_left.y; i <= bot_right.y; ++i)
    {
        for (int j = top_left.x; j <= bot_right.x; ++j)
        {
            auto ss_coord = glm::vec2{get_pixel(j, i).point.x + j, get_pixel(j, i).point.y + i};
            if (tri1.intersects(ss_coord) || tri2.intersects(ss_coord))
            {
                if (get_depth(j, i) > average_depth)
                {
                    paint_pixel(j, i, mpoly.current.color);
                    set_depth(j, i, average_depth);
                }
            }
        }
    }
}

void rys::reyes::render(const rys::Sphere &sphere)
{
    auto mesh = sphere.dice();
    apply_displacement_shader(mesh);

    surface_shader_payload payload;
    if (texture) payload.texture = &(*texture);
    apply_surface_shader(mesh, payload);

    auto grid = mesh.get_grid();

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            auto current = grid[i][j];
            auto right   = grid[i][(j+1) % grid[0].size()];
            auto below   = grid[(i+1)    % grid.size()][j];
            auto cross   = grid[(i+1)    % grid.size()][(j+1) % grid[0].size()];

            auto mpoly = rys::polygon{current, right, below, cross};
            auto bounding_box = find_bounding_box(mpoly);

            paint_intersecting_samples(bounding_box, mpoly);
        }
    }
}

void rys::reyes::print_samples(rys::Mesh& mesh)
{
    for (auto& line : mesh.get_grid())
    {
        for (auto& g : line)
        {
            auto ss = get_ss_coords(g.position);

            bool u_white = (int(g.uv.x * 10) % 2) == 1;
            bool v_white = (int(g.uv.y * 10) % 2) == 1;

            auto val = u_white ^ v_white;
            auto color = glm::vec3{val, val, val};
            paint_pixel(ss.x, ss.y, color);
        }
    }
}

void rys::reyes::render(const rys::Cone &cone)
{
    auto mesh = cone.dice();
//    apply_displacement_shader(mesh);


    surface_shader_payload payload{};
    if (texture) payload.texture = &(*texture);
    apply_surface_shader(mesh, payload);

//    print_samples(mesh);
//
//    return;
    auto grid = mesh.get_grid();

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            auto current = grid[i][j];
            auto right   = grid[i][(j+1) == grid[0].size() ? j : j + 1];
            auto below   = grid[(i+1) == grid.size() ? i : i + 1][j];
            auto cross   = grid[(i+1) == grid.size() ? i : i + 1][(j+1) == grid[0].size() ? j : j + 1];

            auto mpoly = rys::polygon{current, right, below, cross};
            auto bounding_box = find_bounding_box(mpoly);

            paint_intersecting_samples(bounding_box, mpoly);
        }
    }
}

void rys::reyes::render(const rys::Cylinder &cylinder)
{
    auto mesh = cylinder.dice();
//    apply_displacement_shader(mesh);

    surface_shader_payload payload{};
    if (texture) payload.texture = &(*texture);
    apply_surface_shader(mesh, payload);

//    print_samples(mesh);
//
//    return;
    auto grid = mesh.get_grid();

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            auto current = grid[i][j];
            auto right   = grid[i][(j+1) == grid[0].size() ? j : j + 1];
            auto below   = grid[(i+1) == grid.size() ? i : i + 1][j];
            auto cross   = grid[(i+1) == grid.size() ? i : i + 1][(j+1) == grid[0].size() ? j : j + 1];

            auto mpoly = rys::polygon{current, right, below, cross};
            auto bounding_box = find_bounding_box(mpoly);

            paint_intersecting_samples(bounding_box, mpoly);
        }
    }
}

void rys::reyes::render(const rys::Torus &torus)
{
    auto mesh = torus.dice();
//    apply_displacement_shader(mesh);

    surface_shader_payload payload{};
    if (texture) payload.texture = &(*texture);
    apply_surface_shader(mesh, payload);

    auto grid = mesh.get_grid();

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            auto current = grid[i][j];
            auto right   = grid[i][(j+1) == grid[0].size() ? j : j + 1];
            auto below   = grid[(i+1) == grid.size() ? i : i + 1][j];
            auto cross   = grid[(i+1) == grid.size() ? i : i + 1][(j+1) == grid[0].size() ? j : j + 1];

            auto mpoly = rys::polygon{current, right, below, cross};
            auto bounding_box = find_bounding_box(mpoly);

            paint_intersecting_samples(bounding_box, mpoly);
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

void rys::reyes::setup_projection(rys::Projection_Model model, float f)
{
    //TODO : Implement ortho projection!
    proj_model = model;
    fov = f;

    proj_matrix = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);
}

void rys::reyes::initialize_buffers()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    create_frame_buffer();
    create_depth_buffer();
    std::for_each(frame_buffer.begin(), frame_buffer.end(), [&](auto& sample)
    {
        auto x = dist(mt);
        auto y = dist(mt);
        sample.color = glm::vec3{0, 0, 0};  // clear the frame buffer
        sample.point = glm::vec2(x, y);
    });

    std::fill(depth_buffer.begin(), depth_buffer.end(), std::numeric_limits<float>::infinity());
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

bool rys::triangle::intersects(const glm::vec2 &point)
{
    auto sign =[](const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    float d1, d2, d3;
    bool has_neg, has_pos;

    auto v1 = a;
    auto v2 = b;
    auto v3 = c;

    d1 = sign(point, v1, v2);
    d2 = sign(point, v2, v3);
    d3 = sign(point, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

float rys::polygon::get_average_depth() const
{
    return (current.position.z + right.position.z + below.position.z + cross.position.z) / 4.f;
}

void rys::reyes::apply_displacement_shader(rys::Mesh& mesh)
{
    // create the variables that'll be passed to the shader
    displacement_shader(mesh.get_grid()[0][0].position);
}

void rys::reyes::apply_surface_shader(rys::Mesh& mesh, surface_shader_payload& payload)
{
    // create the variables that'll be passed to the shader
    for (auto& line : mesh.get_grid())
    {
        for (auto& grid : line)
        {
            payload.color = glm::vec4(get_color(), 1.0f);
            payload.position = grid.position;
            payload.normal = grid.normal;
            payload.uv = grid.uv;
            surface_shader(payload);

            // output color:
            grid.color = payload.color;
        }
    }
}
