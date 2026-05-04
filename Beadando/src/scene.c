#include "scene.h"
#include <GL/gl.h>
//#include <GL/glu.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>


void init_scene(Scene* scene)
{
    scene->animal_count = 1;
    
    //printf("animal betolt:\n");
    // Rexy hozzáadása
    //Animal* rexy = &(scene->animals[scene->animal_count++]);
    load_model(&(scene->animals[0].model), "assets/models/Trex.obj");
    //printf("animal model ready\n");
    scene->animals[0].texture_id = load_texture("assets/textures/Rexy_Diffuse.png");
    //printf("animal texture ready\n");
    scene->animals[0].position.x = 10.0;
    scene->animals[0].position.y = 10.0;
    scene->animals[0].position.z = 0.0;
    scene->animals[0].rotation_z = 0.0;
    //printf("animal position and rotation ready\n");
    scene->animals[0].speed = 1.0f;
    //printf("animal speed set\n");
    scene->animals[0].turn_speed = 1.0f;
    //printf("animal turn_speed set\n");
    //rexy->is_active = true;
    scene->animals[0].is_sleeping = false;
    scene->animals[0].attak_distance = 8.0f;
    scene->animals[0].attak_damige = .05f;
    //printf("animal is awake\n");

    scene->Player.position.x = 0;
    scene->Player.position.y = 0;
    scene->Player.position.z = 0;

    scene->Player.rotation_z = 0.0f;
    scene->Player.hp = 1.0f;
    scene->Player.speed = 1.5f;

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

        if (animal->is_sleeping)
        {
            continue;
        }

        double dx = camera_pos.x - animal->position.x;
        double dy = camera_pos.y - animal->position.y;
        double distance = sqrt(dx*dx + dy*dy);
        animal->distance_from_player = distance;

        if(animal->distance_from_player > animal->attak_distance)
        {
            animal->position.x += (dx / distance)*animal->speed * elapsed_time;
            animal->position.y += (dy / distance)*animal->speed * elapsed_time;

            float target_angle = atan2(dy,dx) * 180.0 / M_PI -90.0f;

            float angle_diff = target_angle - animal->rotation_z;
            
            
            while (angle_diff < -180)
            {
                angle_diff += 360;
            }

            while (angle_diff > 180)
            {
                angle_diff -= 360;
            }
            
            //printf("target_angle: %f; angle_diff: %f \n",target_angle,angle_diff);
            animal->rotation_z += angle_diff * animal->turn_speed * elapsed_time;
        }

        if (animal->distance_from_player <= animal->attak_distance)
        {
            scene->Player.hp -= animal->attak_damige * elapsed_time;
            //scene->Player.hp -= 5.0 * elapsed_time;
            printf("player hp: %f \n",(scene->Player.hp) *100);
        }
        
        
    }
    scene->Player.position.x = camera_pos.x;
    scene->Player.position.y = camera_pos.y;
    scene->Player.position.z = camera_pos.z;
    
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->animals[0].material));
    set_lighting();
    draw_origin();
    draw_floor();
    //glTranslatef(2,2,1.5); // athelyezes
    //draw_model(&(scene->animals[0].model));

    
    for (int i = 0; i < scene->animal_count; i++)
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

void draw_floor() {
    //glDisable(GL_LIGHTING); // A padló ne legyen árnyékos, hogy mindig lásd az irányokat
    glBegin(GL_QUADS);
    
    // Adjunk neki egy sötétszürke színt
    glColor3f(0.2f, 0.2f, 0.2f); 
    
    // A sarkok koordinátái: -50-től 50-ig az 100 méter szélességet ad ki
    glVertex3f(-50.0f, -50.0f, 0.0f);
    glVertex3f( 50.0f, -50.0f, 0.0f);
    glVertex3f( 50.0f,  50.0f, 0.0f);
    glVertex3f(-50.0f,  50.0f, 0.0f);
    
    glEnd();
    //glEnable(GL_LIGHTING);
}
