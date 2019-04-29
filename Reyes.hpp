#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.hpp"
#include <iostream>
#include <stack>
#include <functional>
#include "glm_ostream.hpp"
#include "shaders.hpp"

namespace rys
{
    struct polygon
    {
        Grid current;
        Grid right;
        Grid below;
        Grid cross;

        float get_average_depth() const;
    };

    struct sample
    {
        glm::vec3 color;
        glm::vec2 point;
    };

    struct triangle
    {
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;

        bool intersects(const glm::vec2& point);
    };

    enum class Display_Type
    {
        Framebuffer,
        Image
    };

    constexpr double pi = 3.14159265359;

    enum class Display_Mode
    {
        RGB,
        RGBA,
        RGBAZ,
        N
    };

    enum class Projection_Model
    {
        Perspective,
        Orthographic
    };

    inline glm::vec4 default_displacement_shader(const glm::vec4& position)
    {
        return position;
    }

    inline void default_surface_shader(surface_shader_payload& payload)
    {
        payload.color = payload.color;
    }

    class reyes
    {
    public:
        reyes() = default;
        reyes(const std::string& n);

        void render(const rys::Sphere& sphere); // for debugging purposes, won't be used for release!
        void paint_pixel(int x, int y, const glm::vec3& color);
        rys::sample get_pixel(int x, int y);
        float get_depth(int x, int y);
        void set_depth(int x, int y, float n_depth);

        glm::mat4 get_current_matrix() const { return current_matrix; }
        void push_current_matrix();
        void pop_current_matrix();

        void save_frame();
        void world_begin();

        void add_transform(const glm::mat4& transform);

        void setup_display(Display_Type type, const std::string& name, Display_Mode mode);
        void setup_projection(Projection_Model model, float f);

        void set_cur_frame_id(int id) { current_frame = id; }
        int get_cur_frame_id() const { return current_frame; }

        void set_pixel_samples(int xsamples, int ysamples);
        std::pair<int, int> get_pixel_samples() const { return std::make_pair(pixel_xsamples, pixel_ysamples); }

        // shaders:
        void set_displacement_shader(std::function<glm::vec4(const glm::vec4&)> ds) { displacement_shader = ds; }
        void set_surface_shader(std::function<void(surface_shader_payload&)> ss) { if (ss) surface_shader = ss; else surface_shader = default_surface_shader; }

        std::vector<sample>& create_frame_buffer();
        std::vector<float>& create_depth_buffer();
        void initialize_buffers();
        void initialize_viewport();

        void set_format(int xres, int yres, float pix_asp_ratio);

        void set_color(const glm::vec3& col) { current_color = col; }
        glm::vec3 get_color() const { return current_color; }

        void set_width(int xres) { width = xres; }
        int  get_width() const { return width; }

        void set_height(int yres) { height = yres; }
        int  get_height() const { return height; }

        void set_aspect_ratio(float ratio) { aspect_ratio = ratio; }
        float get_aspect_ratio() const { return aspect_ratio; }

        void set_viewport_aspect(float ratio) { viewport_aspect_ratio = ratio; }
        float get_viewport_ratio() const { return viewport_aspect_ratio; }

        void set_name(const std::string& n) { name = n;}
        std::string get_name() const { return name; }

    private:
        int current_frame;
        std::string name;

        std::vector<sample> frame_buffer;
        std::vector<float> depth_buffer;

        unsigned int real_width;
        unsigned int real_height;

        unsigned int width;
        unsigned int height;
        float aspect_ratio; // pixel aspect ratio
        float viewport_aspect_ratio;
        float fov;

        int pixel_xsamples;
        int pixel_ysamples;

        Display_Type disp_type;
        Display_Mode disp_mode;
        Projection_Model proj_model;

        glm::mat4 proj_matrix;
        glm::mat4 view_matrix; // world_to_camera!
        glm::mat4 current_matrix;
        glm::mat4 viewport_matrix;

        // shader functions:
        void apply_displacement_shader(Mesh& mesh);
        void apply_surface_shader(Mesh& mesh, surface_shader_payload& payload);
        std::function<glm::vec4(const glm::vec4&)> displacement_shader;
        std::function<void(surface_shader_payload&)> surface_shader;

        std::pair<glm::vec2i, glm::vec2i> find_bounding_box(const rys::polygon& mpoly);

        float near;
        float far;

        bool world_began;

        glm::vec3 current_color;

        std::stack<glm::mat4> transform_stack;

        glm::vec3 get_ss_coords(const glm::vec4& point);
        void paint_intersecting_samples(const std::pair<glm::vec2i, glm::vec2i>& bb, const rys::polygon& mpoly);
    };
}