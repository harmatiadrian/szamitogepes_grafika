#include "scene.h"
#include <GL/gl.h>
//#include <GL/glu.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>


void init_scene(Scene* scene)
{
    scene->animal_count = 1;
    

    // Rexy hozzáadása
    //Animal* rexy = &(scene->animals[scene->animal_count++]);
    load_model(&(scene->animals[0].model), "assets/models/Trex.obj");
    scene->animals[0].texture_id = load_texture("assets/textures/Rexy_Diffuse.png");
    scene->animals[0].position.x = 2.0;
    scene->animals[0].position.y = 2.0;
    scene->animals[0].position.z = 0.0;
    scene->animals[0].speed = .5;
    //rexy->is_active = true;
    scene->animals[0].is_sleeping = false;

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

        if(distance > 1.5)
        {
            animal->position.x += (dx / distance)*animal->speed * elapsed_time;
            animal->position.y += (dy / distance)*animal->speed * elapsed_time;
        }
        
    }
    
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
        printf("dinoKord: %f,%f,%f\n",scene->animals[i].position.x,scene->animals[i].position.y,scene->animals[i].position.z);
        const Animal* animal = &(scene->animals[i]);

        glPushMatrix();
            glTranslatef(animal->position.x, animal->position.y, animal->position.z);

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
