#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_ENTITIES 10

/*
typedef struct {
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    float rotation_z;
    float speed;
    bool is_active;
} Entity;
*/
typedef struct {
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    float rotation_z;
    float speed;
    float alertness; // Mennyire veszi észre a játékost
    bool is_sleeping; // Altatás állapota
    float distance_from_player;
    float turn_speed;
    float attak_distance;
    float attak_damige;
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
} Player;


typedef struct Scene
{
    Animal animals[MAX_ENTITIES];
    int animal_count;
    Material material; // Globális anyagbeállításokhoz
    Player Player;
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

#endif /* SCENE_H */
