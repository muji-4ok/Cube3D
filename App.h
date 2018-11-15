#pragma once
#include "Shader.h"
#include "Buffers.h"
#include "Cube.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <algorithm>
#include <cmath>
#include <random>
#include <ctime>

enum Rotation_Dir
{
    DIR1 = 0,
    DIR2 = 1
};

struct App
{
public:
    App(int width, int height);
    ~App();

    void error_callback(int, const char *);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void frame_buffer_change_callback(GLFWwindow *window, int width, int height);
    void mouse_callback(GLFWwindow *window, int button, int action, int mods);

private:
    void run();

    void prepare();
    void calculate();
    void draw();
    void update_fps();

    bool hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                  int &index, int &hit_i, int &hit_j, int &hit_k);
    float get_rotation_angle(int index, int i, int j, int k, Rotation_Dir dir, const glm::vec2 &div_vec,
                             const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos,
                             glm::vec3 &rotation_vec);
    void get_pane_vertices(int index, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
    void get_rot_pane_vertices(int index, int i, int j, int k, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
    glm::vec3 get_vert_by_coord(int i, int j, int k);
    glm::vec3 get_rot_vec(int index, int i, int j, int k, Rotation_Dir dir);
    glm::vec3 get_rotation_vec(int index);
    bool needs_rotation(int index, Rotation_Dir dir, int hit_i, int hit_j, int hit_k, int i, int j, int k);
    Rotation_Dir get_dir(int index, const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos,
                         glm::vec2 &vec_dir);
    int get_rot_index(int index, Rotation_Dir dir);
    glm::vec3 project_point_onto_plane(const glm::vec3 &plane_point, const glm::vec3 &plane_normal,
                                       const glm::vec3 &point);
    bool are_similary_oriented(const glm::vec3 &stable , const glm::vec3 &unstable);
    bool needs_fixing(int index, int rot_index);
    glm::vec3 get_index_normal(int index);
    glm::mat4 get_mat();
    std::array<std::vector<float>*, 8> get_rotating_cubelets(int index, Rotation_Dir dir,
                                                             int hit_i, int hit_j, int hit_k);
    Cubelet_Rotation get_cubelet_rotation(int index);
    void rotate(int index, Rotation_Dir dir, int turns, int hit_i, int hit_j, int hit_k);
    void shuffle();

    cv::VideoCapture vcap;
    bool with_webcam;

    GLFWwindow *window;
    int width;
    int height;

    Shader vertex;
    Shader fragment;
    ShaderProgram shdProgram;
    VBO cubeVBO;
    VAO cubeVAO;

    Cube cube;
    glm::mat4 projection;
    glm::mat4 view;

    float mouse_dist = 0.0f;
    float increment = 0.0f;
    int turns = 0;
    float auto_rotation_dir = 0.0f;
    float final_angle = 0.0f;
    bool auto_rotating = false;
    bool rotating = false;
    glm::vec2 last_mouse_pos;
    bool has_dir = true;
    bool has_rotation_vec = true;

    float rot_angle = 0.0f;
    Rotation_Dir dir = DIR1;

    int hit_index;
    int hit_i, hit_j, hit_k;

    bool mouse_pressed = false;

    bool right_mouse_pressed = false;
    glm::vec2 last_right_mouse_pos;

    glm::mat4 mat = glm::mat4(1.0f);
    glm::mat4 cur_mat = glm::mat4(1.0f);
    glm::mat4 original_view = glm::mat4(1.0f);

    glm::mat4 translate_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    bool is_cur_good = false;

    glm::vec3 obj_position;

    const std::vector<float> square_vertices{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };

    // Shader squareVertex;
    // Shader squareFragment;
    // ShaderProgram squareShdProgram;
    // VBO squareVBO;
    // VAO squareVAO;
};

struct CallbackCaller
{
    static App *app;

    static void error_callback_caller(int, const char *);
    static void key_callback_caller(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void frame_buffer_change_callback_caller(GLFWwindow *window, int width, int height);
    static void mouse_callback_caller(GLFWwindow *window, int button, int action, int mods);
};

float dist_from_ray_to_triangle(const glm::vec3 &origin, const glm::vec3 &dir,
                                const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);
glm::vec3 get_eye_ray(double x_pos, double y_pos, int width, int height,
                      const glm::mat4 &projection, const glm::mat4 &view);
glm::vec3 get_eye_ray(const glm::vec2 &mouse_pos, const glm::mat4 &projection, const glm::mat4 &view);
glm::vec2 get_mouse_pos(double x_pos, double y_pos, int width, int height);
float intersect_ray_plane(const glm::vec3 &normal, const glm::vec3 &plane_point, const glm::vec3 &ray_origin,
                          const glm::vec3 &ray_dir);
