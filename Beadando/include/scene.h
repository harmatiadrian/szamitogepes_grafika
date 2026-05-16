#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_ENTITIES 10
#define MAX_ANOMALIES 1
#define MAX_ANIMAL_COUNT 10

typedef enum Anomaly_Leading_Time
{
    Precambrian = 0,
    Silurian = 1,
    Carboniferous = 2,
    Permian = 3,
    Triassic = 4,
    Jurassic = 5,
    Cretaceous = 6,
    Eocene = 7,
    Pliocene = 8,
    Holocene = 9,
    Future = 10,
} Anomaly_Leading_Time;

typedef struct entity
{
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    float rotation_z;
} Entity;

typedef struct {
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    float rotation_z;
    float speed;
    //float alertness;
    bool is_sleeping; // Altatás állapota
    float distance_from_player;
    float turn_speed;
    float attak_distance;
    float attak_damige;
    float stamina;
    float base_speed;
    bool is_running;
    float sleeping_time;
} Animal;

typedef struct scene
{
    /*
    Model model;
    Material material;
    GLuint texture_id;
    */
    vec3 position;
    float rotation_z;
    float speed;
    float hp;
    float stamina;
    float base_speed;
    bool is_running;
} Player;

typedef struct
{
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    bool is_open;
    Anomaly_Leading_Time leading_time;

} Anomaly;


typedef struct Scene
{
    Animal animals[MAX_ANIMAL_COUNT];
    int animal_count;
    Anomaly anomalies[MAX_ANOMALIES];
    int anomaly_count;
    Entity entities[MAX_ENTITIES];
    int entity_count;
    Material material; // Globális anyagbeállításokhoz
    Player Player;
    bool is_paused;
    bool is_help_visible;
    GLuint help_texture_id;
    bool is_main_menu_visible;
    GLuint main_menu_texture_id;
} Scene;



/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, vec3 camera_pos, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

/**
 * Draw the floor of the scene.
 */
void draw_floor();

void draw_wire_sphere(vec3 center, float radius);

void sleep_nerby_creatures(Scene* scene);

void draw_wire_box(vec3 position, float half_x, float half_y);

void render_anomaly(const Anomaly* anomaly);

#endif /* SCENE_H */
