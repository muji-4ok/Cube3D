#pragma once
#include "Common.h"
#include "CubeModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <utility>
#include <memory>


class CubeMouseHitter
{
public:
    CubeMouseHitter(CubeModel* cm) : cubeModel(cm) {}

    bool get_hit_position(const glm::vec2& mouse_pos, HitHeader& hit) const;

private:
    const CubeModel* cubeModel;

    glm::vec3 get_eye_ray(const glm::vec2& mouse_pos) const;
    bool hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                  HitHeader& hit) const;
    float dist_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir,
                               const glm::vec3 & v0, const glm::vec3 & v1,
                               const glm::vec3 & v2) const;

};

class Rotater
{
public:
    Rotater() {}
    Rotater(CubeModel* cm) : cubeModel(cm) {}
    void process_queue();

protected:
    glm::vec3 get_vert_by_coord(int i, int j, int k) const;
    void get_pane_vertices(int index, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3) const;

    glm::vec3 get_rot_vec(int index, RotationDir dir) const;
    int get_rot_index(int index, RotationDir dir) const;
    glm::vec3 get_index_normal(int index) const;

    void generate_temp_rotations(float start_angle, float end_angle, float start_speed,
                                 float acceleration, const glm::vec3 & vec);

    CubeModel* cubeModel;

private:
    void process_temp(const TempRotationHeader* header);
    void process_perm(const PermRotationHeader* header);
    void process_hit(const SetHitRotationHeader* header);
    void process_hit_pos(const SetHitPosRotationHeader* header);
    void process_hit_dir(const SetHitDirRotationHeader* header);
    void process_whole_rotation(const WholeRotationHeader* header);

    bool needs_rotation(int i, int j, int k) const;
    std::array<Cubelet*, 8> get_rotating_cubelets() const;
    RotationAxis get_cubelet_rotation(int rot_index) const;

};

class ScriptRotater : virtual public Rotater
{
public:
    ScriptRotater() {}
    ScriptRotater(CubeModel* cm) : Rotater(cm) {}
    void rotate_script(char r);
    void rotate_all_script(RotationAxis axis, bool clockwise);
    
private:
    HitHeader get_hit_header(char r) const;
    void get_hit_pos(char r, HitHeader& hit) const;
    void get_hit_dir(char r, HitHeader& hit) const;
    bool is_clockwise(char r) const;
};

class InteractiveRotater : virtual public Rotater
{
public:
    InteractiveRotater()
    {
        mouse_hitter = std::make_unique<CubeMouseHitter>(cubeModel);
    }
    InteractiveRotater(CubeModel* cm) : Rotater(cm)
    {
        mouse_hitter = std::make_unique<CubeMouseHitter>(cm);
    }

    void rotate_interactive(const glm::vec2& mouse_diff);
    void set_interactive_dir(const glm::vec2& mouse_diff);
    void set_interactive_pos(const glm::vec2& mouse_pos);
    void finish_interactive_rotation_snap(float angle, HitHeader hit, TempRotationHeader lastRotation);
    void finish_interactive_rotation();
    void finish_interactive_rotation_to_zero();
    bool is_going_to_rotate_finish_interactive();

    void rotate_all_interactive(const glm::vec2 &mouse_diff);

private:
    std::unique_ptr<CubeMouseHitter> mouse_hitter;

};

class HybridRotater : virtual public ScriptRotater, virtual public InteractiveRotater
{
public:
    HybridRotater(CubeModel* cm) : Rotater(cm), ScriptRotater(cm), InteractiveRotater(cm) {}

};