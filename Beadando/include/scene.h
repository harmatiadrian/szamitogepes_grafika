#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_ENTITIES 10

typedef struct Scene
{
    Entity entities[MAX_ENTITIES];
    int entity_count;
    Material material; // Globális anyagbeállításokhoz
} Scene;

typedef struct {
    Model model;
    Material material;
    GLuint texture_id;
    vec3 position;
    float speed;
    float alertness; // Mennyire veszi észre a játékost
    bool is_sleeping; // Altatás állapota
} Animal;

typedef struct {
    Model model;
    GLuint texture_id;
    vec3 position;
    float rotation_z;
    float speed;
    bool is_active;
} Entity;

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
void update_scene(Scene* scene);

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
