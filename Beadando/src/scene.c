#if defined(_WIN32) || defined(_WIN64)
    #define _CRT_RAND_S
#elif defined(__linux__)
    #include <sys/random.h>
#else
    #include <time.h>
#endif

#include "scene.h"
#include <GL/gl.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#define ROCK_HALF_Y (4.0f)
#define ROCK_HALF_X (3.0f)

#define TREX_RADIUS (1.1f)

#define PLAYER_RADIUS (0.5f)

static double random_between(double min, double max)
{
    #if defined(_WIN32) || defined(_WIN64)
        unsigned int value;
        if (rand_s(&value) != 0) {
            value = 0;
        }
        return min + (double)value / (double)UINT_MAX * (max - min);
    #elif defined(__linux__)
        unsigned int value = 0;
        if (getrandom(&value, sizeof(value), 0) != (ssize_t)sizeof(value)) {
            value = (unsigned int)time(NULL);
        }
        return min + (double)value / (double)UINT_MAX * (max - min);
    #else
        return min + (double)rand() / (double)RAND_MAX * (max - min);
    #endif
}

void init_scene(Scene* scene)
{
    scene->animal_count = 1;
    scene->anomaly_count = 1;
    scene->entity_count = 2;

    #if !defined(_WIN32) && !defined(_WIN64) && !defined(__linux__)
        srand((unsigned)time(NULL));
    #endif

    load_model(&(scene->animals[0].model), "assets/models/Trex.obj");
    load_model(&(scene->entities[0].model), "assets/models/Long Boulder.obj");
    load_model(&(scene->entities[1].model), "assets/models/Long Boulder.obj");
    scene->animals[0].texture_id = load_texture("assets/textures/Rexy_Diffuse.png",false);
    scene->animals[0].position.x = 10.0;
    scene->animals[0].position.y = 10.0;
    scene->animals[0].position.z = 0.0;
    scene->animals[0].rotation_z = 0.0;
    scene->animals[0].speed = 1.0f;
    scene->animals[0].base_speed = 1.0f;
    scene->animals[0].turn_speed = 1.0f;
    scene->animals[0].is_sleeping = false;
    scene->animals[0].attak_distance = 8.0f;
    scene->animals[0].attak_damige = .2f;
    scene->animals[0].stamina = 50.0f;
    scene->animals[0].is_running = false;
    scene->animals[0].sleeping_time = 0.0f;

    scene->entities[0].position.x = random_between(-40.0, 40.0);
    scene->entities[0].position.y = random_between(-40.0, 40.0);
    scene->entities[0].position.z = -0.4f;
    scene->entities[0].rotation_z = 0.0f;
    scene->entities[0].texture_id = load_texture("assets/textures/Long Boulder.png",false);

    scene->entities[1].position.x = random_between(-40.0, 40.0);
    scene->entities[1].position.y = random_between(-40.0, 40.0);
    scene->entities[1].position.z = -0.4f;
    scene->entities[1].rotation_z = 0.0f;
    scene->entities[1].texture_id = load_texture("assets/textures/Long Boulder.png",false);
    
    scene->anomalies[0].position.x = random_between(-40.0, 40.0);
    scene->anomalies[0].position.y = random_between(-40.0, 40.0);
    scene->anomalies[0].position.z = 1.8f;
    scene->anomalies[0].texture_id = load_texture("assets/textures/anomaly.png",true);
    
    scene->Player.position.x = 0;
    scene->Player.position.y = 0;
    scene->Player.position.z = 0;
    scene->Player.rotation_z = 0.0f;
    scene->Player.hp = 1.0f;
    scene->Player.speed = 1.5f;
    scene->Player.base_speed = 1.5f;
    scene->Player.stamina = 100.0f;
    scene->Player.is_running = false;

    // Alapértelmezett anyagtulajdonságok
    scene->material.ambient = (Color){1.0, 1.0, 1.0};
    scene->material.diffuse = (Color){1.0, 1.0, 1.0};
    scene->material.specular = (Color){0.0, 0.0, 0.0};
    scene->material.shininess = 0.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 50.0f, 50.0f, 80.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

static double clamp_double(double value, double min, double max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, vec3 camera_pos, double elapsed_time)
{

    for (int i = 0; i < scene->animal_count; i++)
    {
        Animal* animal = &(scene->animals[i]);
        if (animal->sleeping_time < 0)
        {
            animal->is_sleeping = false;
            animal->sleeping_time = 0;
        }
        if (animal->is_sleeping)
        {
            animal->sleeping_time -= 1.0f * elapsed_time;
            //printf("sleep: %f \n",animal->sleeping_time);
            continue;
        } 

        double dx = camera_pos.x - animal->position.x;
        double dy = camera_pos.y - animal->position.y;
        double distance = sqrt(dx*dx + dy*dy);
        animal->distance_from_player = distance;

        if (animal->distance_from_player > 7.0f && distance > 1e-6)
        {
            float next_x = animal->position.x + (dx / distance) * animal->speed * elapsed_time;
            float next_y = animal->position.y + (dy / distance) * animal->speed * elapsed_time;

            bool collision = false;
            for (int j = 0; j < scene->entity_count; j++) {
                Entity* rock = &(scene->entities[j]);
                
                float b_min_x = rock->position.x - ROCK_HALF_X;
                float b_max_x = rock->position.x + ROCK_HALF_X;
                float b_min_y = rock->position.y - ROCK_HALF_Y;
                float b_max_y = rock->position.y + ROCK_HALF_Y;

                float closest_x = clamp_double(next_x, b_min_x, b_max_x);
                float closest_y = clamp_double(next_y, b_min_y, b_max_y);

                float dist_x = next_x - closest_x;
                float dist_y = next_y - closest_y;

                if ((dist_x*dist_x + dist_y*dist_y) < (TREX_RADIUS * TREX_RADIUS)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                animal->position.x = next_x;
                animal->position.y = next_y;
            }
        }

        float target_angle = atan2(dy, dx) * 180.0 / M_PI - 90.0f;
        float angle_diff = target_angle - animal->rotation_z;
        while (angle_diff < -180) angle_diff += 360;
        while (angle_diff > 180) angle_diff -= 360;
        animal->rotation_z += angle_diff * animal->turn_speed * elapsed_time;

        if (animal->distance_from_player <= animal->attak_distance)
        {
            if (scene->Player.hp > 0)
            {
                scene->Player.hp -= animal->attak_damige * elapsed_time;
            }
            else
            {
                scene->Player.hp = 0;
            }
            printf("player hp: %f \n",(scene->Player.hp) *100);
        }
        //animal->stamina -= 0.1f * elapsed_time;
    }
    scene->Player.position.x = camera_pos.x;
    scene->Player.position.y = camera_pos.y;
    scene->Player.position.z = camera_pos.z;
    //printf("player stamina: %f \n player position: %f, %f, %f",(scene->Player.stamina), scene->Player.position.x, scene->Player.position.y, scene->Player.position.z);
    //printf("animal distance: %f ; animal sleeping: %d\n",scene->animals[0].distance_from_player, scene->animals[0].is_sleeping);


    if (scene->Player.is_running) {
        scene->Player.stamina -= 1.0f * elapsed_time;
        if (scene->Player.stamina < 0) {
            scene->Player.stamina = 0;
            scene->Player.speed = scene->Player.base_speed;
            scene->Player.is_running = false;
        }
    } else {
        scene->Player.stamina += 0.5f * elapsed_time;
        if (scene->Player.stamina > 100.0f) {
            scene->Player.stamina = 100.0f;
        }
    }

    scene->Player.position.x = clamp_double(scene->Player.position.x, -50.0, 50.0);
    scene->Player.position.y = clamp_double(scene->Player.position.y, -50.0, 50.0);
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->animals[0].material));
    set_lighting();
    draw_origin();
    draw_floor();

    int i;
    
    for (i = 0; i < scene->animal_count; i++)
    {
        //printf("dinoKord: %f,%f,%f; tavolsag: %f\n",scene->animals[i].position.x,scene->animals[i].position.y,scene->animals[i].position.z, scene->animals[i].distance_from_player);
        const Animal* animal = &(scene->animals[i]);

        glPushMatrix();
            glTranslatef(animal->position.x, animal->position.y, animal->position.z);

            glRotatef(animal->rotation_z, 0.0f, 0.0f, 1.0f);

            glBindTexture(GL_TEXTURE_2D, animal->texture_id);
            draw_model(&(animal->model));
        glPopMatrix();
    }
    
    for (i = 0; i < scene->entity_count; i++)
    {
        const Entity* entity = &(scene->entities[i]);

        glPushMatrix();
            glTranslatef(entity->position.x, entity->position.y, entity->position.z);

            glRotatef(entity->rotation_z, 0.0f, 0.0f, 1.0f);

            glBindTexture(GL_TEXTURE_2D, entity->texture_id);
            draw_model(&(entity->model));
        glPopMatrix();
    }
    
    for (i = 0; i < scene->anomaly_count; i++)
    {
        render_anomaly(&(scene->anomalies[i]));
    }
    

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f); 

    glColor3f(1.0f, 0.0f, 0.0f); 
    for (int i = 0; i < scene->animal_count; i++) {
        draw_wire_sphere(scene->animals[i].position, TREX_RADIUS);
    }

    glColor3f(0.0f, 1.0f, 0.0f); 
    for (int i = 0; i < scene->entity_count; i++) {
        vec3 visual_pos = {
            scene->entities[i].position.x,
            scene->entities[i].position.y,
            scene->entities[i].position.z
        };
        draw_wire_box(visual_pos, ROCK_HALF_X, ROCK_HALF_Y);
    }
    
    glColor3f(0.0f, 0.0f, 1.0f); 
    draw_wire_sphere(scene->Player.position, PLAYER_RADIUS);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glLineWidth(1.0f);
}

void draw_origin()
{
    glEnable(GL_COLOR_MATERIAL);
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glColor3f(1, 1, 1);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
}

void draw_floor() 
{
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    
    glColor3f(0.45f, 0.33f, 0.18f);
    
    glVertex3f(-50.0f, -50.0f, 0.0f);
    glVertex3f( 50.0f, -50.0f, 0.0f);
    glVertex3f( 50.0f,  50.0f, 0.0f);
    glVertex3f(-50.0f,  50.0f, 0.0f);
    

    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LIGHTING);
}

void sleep_nerby_creatures(Scene* scene)
{
    for (int i = 0; i < scene->animal_count; i++)
    {
        Animal* animal = &(scene->animals[i]);
        if (animal->distance_from_player < 8.0f) {
            animal->is_sleeping = true;
            animal->sleeping_time = 10.0f;
        }
    }
}

void draw_wire_sphere(vec3 center, float radius) {
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);
    
    int lats = 10;
    int longs = 10;
    for (int i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = radius * sin(lat0);
        double zr0 = radius * cos(lat0);

        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = zr0 * cos(lng);
            double y = zr0 * sin(lng);
            glVertex3f(x, y, z0);
        }
        glEnd();
    }
    
    for (int j = 0; j <= longs; j++) {
        double lng = 2 * M_PI * (double) (j - 1) / longs;
        double x = cos(lng);
        double y = sin(lng);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= lats; i++) {
            double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
            double z0  = radius * sin(lat0);
            double zr0 = radius * cos(lat0);
            glVertex3f(x * zr0, y * zr0, z0);
        }
        glEnd();
    }

    glPopMatrix();
}

void draw_wire_box(vec3 position, float half_x, float half_y) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    
    float half_z = 0.725f; // 1.45 / 2

    glBegin(GL_LINE_LOOP);
        glVertex3f(-half_x, -half_y, -half_z);
        glVertex3f( half_x, -half_y, -half_z);
        glVertex3f( half_x,  half_y, -half_z);
        glVertex3f(-half_x,  half_y, -half_z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(-half_x, -half_y,  half_z);
        glVertex3f( half_x, -half_y,  half_z);
        glVertex3f( half_x,  half_y,  half_z);
        glVertex3f(-half_x,  half_y,  half_z);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(-half_x, -half_y, -half_z); glVertex3f(-half_x, -half_y,  half_z);
        glVertex3f( half_x, -half_y, -half_z); glVertex3f( half_x, -half_y,  half_z);
        glVertex3f( half_x,  half_y, -half_z); glVertex3f( half_x,  half_y,  half_z);
        glVertex3f(-half_x,  half_y, -half_z); glVertex3f(-half_x,  half_y,  half_z);
    glEnd();

    glPopMatrix();
}

void render_anomaly(const Anomaly* anomaly)
{
    glPushMatrix();

    glTranslatef(anomaly->position.x, anomaly->position.y, anomaly->position.z);

    glRotatef(90.0f,1.0f,0.0f,0.0f);

    static float angle = 0;
    angle +=0.5f;
    glRotatef(angle,0.0f,0.0f,1.0f);

    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f,1.0f,1.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, anomaly->texture_id);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,0.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,0.0f);
        glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f,1.0f,0.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,0.0f);
    glEnd();

    
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();

}
