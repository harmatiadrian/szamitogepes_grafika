#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    scene->entity_count = 0;

    // Rexy hozzáadása
    Entity* rexy = &(scene->entities[scene->entity_count++]);
    load_model(&(rexy->model), "assets/models/Trex.obj");
    rexy->texture_id = load_texture("assets/textures/Rexy_Diffuse.png");
    rexy->position = (vec3){10.0, 10.0, 0.0};
    rexy->speed = 2.5;
    rexy->is_active = true;

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

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    set_material(&(Animal->material));
    set_lighting();
    draw_origin();
    draw_floor();
    //glTranslatef(2,2,1.5); // athelyezes
    draw_model(&(Animal->model));
    
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(4, 0, 0);

    // 4X4 
    /*
    for (size_t i = 1; i < 4; i++)
    {
        for (size_t j = 1; j < 4; j++)
        {
            glColor3f(1, 0, 0);
            glVertex3f(0, i, j);
            glVertex3f(4, i, j);
        }
    }
    */

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 4, 0);

    //4x4
    /*
    for (size_t i = 1; i < 4; i++)
    {
        for (size_t j = 1; j < 4; j++)
        {
            glColor3f(1, 0, 0);
            glVertex3f(i, 0, j);
            glVertex3f(i, 4, j);
        }
    }
    */

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 4);

    //4x4
    /*
    for (size_t i = 1; i < 4; i++)
    {
        for (size_t j = 1; j < 4; j++)
        {
            glColor3f(1, 0, 0);
            glVertex3f(i, j, 0);
            glVertex3f(i, j, 4);
        }
    }
    */
    glEnd();
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
