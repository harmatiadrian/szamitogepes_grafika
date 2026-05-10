#include "app.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>

#define ROCK_OFFSET_X (-3.85f)
#define ROCK_OFFSET_Y (2.55f)
#define TREX_OFFSET_Y (5.5f)

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;
    app->uptime = 0;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Primeval Project",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 100
    );
}

static double clamp_double(double value, double min, double max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void handle_app_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), app->scene.Player.speed);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -(app->scene.Player.speed));
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), app->scene.Player.speed);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -(app->scene.Player.speed));
                break;
            case SDL_SCANCODE_LSHIFT:
                if (app->scene.Player.stamina > 0) 
                {
                    app->scene.Player.speed = app->scene.Player.speed * 2;
                    app->scene.Player.is_running = true;
                }
                break;
            case SDL_SCANCODE_SPACE:
                sleep_nerby_creatures(&(app->scene));
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_LSHIFT:
                app->scene.Player.speed = app->scene.Player.base_speed;
                app->scene.Player.is_running = false;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time = (double)SDL_GetTicks() / 1000;
    double elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    // 1. Mentjük el, hol voltunk, mielőtt elmozdultunk
    vec3 old_pos = app->camera.position;

    // 2. A kamera kiszámolja az új pozíciót az input (W,A,S,D) alapján
    update_camera(&(app->camera), elapsed_time);

    // 3. Ütközésvizsgálat a sziklákkal
    bool player_hit = false;
    for (int i = 0; i < app->scene.entity_count; i++) {
        Entity* rock = &(app->scene.entities[i]);

        // Szikla határai
        float b_min_x = rock->position.x + ROCK_OFFSET_X - ROCK_HALF_X;
        float b_max_x = rock->position.x + ROCK_OFFSET_X + ROCK_HALF_X;
        float b_min_y = rock->position.y + ROCK_OFFSET_Y - ROCK_HALF_Y;
        float b_max_y = rock->position.y + ROCK_OFFSET_Y + ROCK_HALF_Y;

        // Legközelebbi pont a sziklán a játékoshoz
        float closest_x = clamp_double(app->camera.position.x, b_min_x, b_max_x);
        float closest_y = clamp_double(app->camera.position.y, b_min_y, b_max_y);

        float dist_x = app->camera.position.x - closest_x;
        float dist_y = app->camera.position.y - closest_y;
        float dist_sq = (dist_x * dist_x) + (dist_y * dist_y);

        if (dist_sq < (PLAYER_RADIUS * PLAYER_RADIUS)) {
            player_hit = true;
            break;
        }
    }

    // 4. Ha ütköztünk, rakjuk vissza a játékost a régi helyére
    if (player_hit) {
        app->camera.position = old_pos;
    }

    for (int i = 0; i < app->scene.animal_count; i++) {
        Animal* animal = &(app->scene.animals[i]);
        
        // Kiszámoljuk a távolságot a játékos és a dínó között
        float dx = app->camera.position.x - animal->position.x;
        float dy = app->camera.position.y - animal->position.y;
        float dist_sq = dx*dx + dy*dy;

        // Ha túl közel vagyunk (pl. 2.0 egység), nem engedjük tovább a játékost
        float min_dist = 2.0f; 
        if (dist_sq < (min_dist * min_dist)) {
            app->camera.position = old_pos; // Visszaugrik a régi helyre
        }
    }

    app->camera.position.x = clamp_double(app->camera.position.x, -50.0, 50.0);
    app->camera.position.y = clamp_double(app->camera.position.y, -50.0, 50.0);

    // Frissítjük az összes entitást a játékos helyzete alapján
    update_scene(&(app->scene), app->camera.position, elapsed_time);
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    //glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
