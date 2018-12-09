#pragma once
#include "Shader.h"
#include "Buffers.h"
#include "Common.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include <limits>
#include <memory>

class CubeModel
{
public:
    CubeModel();
    ~CubeModel();

    void generate(int width, int height);
    void set_dimensions(int width, int height);
    void draw();
    void rotate_all_with_mouse(const glm::vec2 &diff);
    void rotate_facelet(const HitHeader &hit, const TempRotationHeader* rot);
    bool get_hit_header(const glm::vec2 &mouse_pos, HitHeader &hit);
    void get_dir(const HitHeader &hit, const glm::vec2 &diff, glm::vec2 &dir_vec, Rotation_Dir &dir);
    void get_dir_by_notation(const HitHeader &hit, char r, glm::vec2 &dir_vec, Rotation_Dir &dir);
    std::unique_ptr<TempRotationHeader> get_rotation_header(const glm::vec2 &diff, const HitHeader &hit);
    void rotate_permanent(const PermRotationHeader *perm);
    void reset_models_rotations();
    HitHeader notation_to_hit_header(char r);
    std::vector<std::unique_ptr<RotationHeader>> get_rotations_to_orthogonal(
        float angle, const HitHeader &hit,
        const TempRotationHeader &lastRotation
    );

private:
    void reset_models_translations();
    void set_solved();
    bool needs_rotation(const HitHeader &hit, int i, int j, int k);
    glm::vec3 get_eye_ray(const glm::vec2& mouse_pos);
    bool hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray, int &index,
                  int &hit_i, int &hit_j, int &hit_k);
    float dist_ray_to_plane(const glm::vec3 &ray_origin, const glm::vec3 &ray_dir, const glm::vec3 &plane_point,
                            const glm::vec3 &plane_normal);
    float dist_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir,
                               const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2);
    void get_pane_vertices(int index, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3);
    glm::vec3 get_vert_by_coord(int i, int j, int k);
    glm::vec3 get_rot_vec(const HitHeader &hit);
    int get_rot_index(const HitHeader &hit);
    bool needs_fixing(int index, int rot_index);
    std::array<std::vector<float>*, 8> get_rotating_cubelets(const HitHeader &hit);
    CubeletRotation get_cubelet_rotation(const HitHeader &hit);
    void rotate_cubelet(std::vector<float>& cubelet, CubeletRotation dir);
    std::vector<std::unique_ptr<RotationHeader>> generate_rotations(float start_angle,
                                                                    float end_angle,
                                                                    float start_speed,
                                                                    float acceleration,
                                                                    const glm::vec3 & vec);

    const std::vector<float> raw_vertices{
        // back
        -0.5f, -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,

        // front
        -0.5f, -0.5f,  0.5f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,

        // left
        -0.5f,  0.5f,  0.5f, 2.0f,
        -0.5f,  0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f,  0.5f, 2.0f,
        -0.5f,  0.5f,  0.5f, 2.0f,

        // right
         0.5f,  0.5f,  0.5f, 3.0f,
         0.5f,  0.5f, -0.5f, 3.0f,
         0.5f, -0.5f, -0.5f, 3.0f,
         0.5f, -0.5f, -0.5f, 3.0f,
         0.5f, -0.5f,  0.5f, 3.0f,
         0.5f,  0.5f,  0.5f, 3.0f,

         // bottom
        -0.5f, -0.5f, -0.5f, 4.0f,
         0.5f, -0.5f, -0.5f, 4.0f,
         0.5f, -0.5f,  0.5f, 4.0f,
         0.5f, -0.5f,  0.5f, 4.0f,
        -0.5f, -0.5f,  0.5f, 4.0f,
        -0.5f, -0.5f, -0.5f, 4.0f,

        // top
        -0.5f,  0.5f, -0.5f, 5.0f,
         0.5f,  0.5f, -0.5f, 5.0f,
         0.5f,  0.5f,  0.5f, 5.0f,
         0.5f,  0.5f,  0.5f, 5.0f,
        -0.5f,  0.5f,  0.5f, 5.0f,
        -0.5f,  0.5f, -0.5f, 5.0f
    };
    const std::vector<glm::vec3> normals{
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    const std::vector<glm::vec3> vertices{
        // back
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // front
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        // left
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        // right
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),

         // bottom
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // top
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f)
    };

    std::array<std::array<std::array<std::vector<float>, 3>, 3>, 3 > colors;
    std::array<std::array<std::array<glm::mat4, 3>, 3>, 3 > models_rotate;
    std::array<std::array<std::array<glm::mat4, 3>, 3>, 3 > models_translate;
    std::array<std::array<std::array<glm::mat4, 3>, 3>, 3 > models_translate_full;

    const std::string vertex_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)";
    const std::string fragment_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)";
    ShaderProgram shdProgram;
    VBO cubeVBO;
    VAO cubeVAO;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 rotation_view;
    glm::mat4 translation_view;

    int width;
    int height;
};

template <typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2);

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2);
