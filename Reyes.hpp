#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.hpp"
#include <iostream>
#include <stack>
#include "glm_ostream.hpp"

namespace rys
{
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

    class reyes
    {
    public:
        reyes() = default;
        reyes(const std::string& n);

        void render(const rys::Sphere& sphere); // for debugging purposes, won't be used for release!
        void paint_pixel(int x, int y, const glm::vec3& color);

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

        void set_pixel_samples(int xsamples, int ysamples) { pixel_xsamples = xsamples; pixel_ysamples = ysamples; }
        std::pair<int, int> get_pixel_samples() const { return std::make_pair(pixel_xsamples, pixel_ysamples); }

        std::vector<glm::vec3>& get_frame_buffer();

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

        std::vector<glm::vec3> frame_buffer;

        int width;
        int height;
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

        float near;
        float far;

        bool world_began;

        glm::vec3 current_color;

        std::stack<glm::mat4> transform_stack;
    };
}