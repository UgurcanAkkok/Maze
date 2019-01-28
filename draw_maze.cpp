#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

#include "./maze_gen.cpp"

#define DEBUG false

void Cell::draw(int x, int y){
    ALLEGRO_COLOR color_wall = al_map_rgb_f(1,1,1);
    if (this->tracked){
        color_wall = al_map_rgb_f(1,0,1);
    }
    if (this->right_open == false){
        al_draw_line(x + CELL_SIZE, y, x + CELL_SIZE, y + CELL_SIZE,color_wall,1);
    }
    if (this->left_open == false){
        al_draw_line(x, y, x, y + CELL_SIZE,color_wall,1);
    }
    if (this->up_open == false){
        al_draw_line(x, y, x + CELL_SIZE, y,color_wall,1);
    }
    if (this->down_open == false){
        al_draw_line(x, y + CELL_SIZE, x + CELL_SIZE, y + CELL_SIZE,color_wall,1);
    }

}

void Maze::draw(int x, int y){
    for (int i = 0; i < MAZE_SIZE; i++){
        for (int j = 0; j < MAZE_SIZE; j++){
            this->maze_map[i][j].draw(x + i * CELL_SIZE, y + j * CELL_SIZE);
        }
    }

}

void init(bool content, const char * name){
   if (content)
       return;
   else {
       std::cout << name << "could not be initialized" << std::endl;
   }

}
int main(){
    init(al_init(),"allegro");
    
    ALLEGRO_TIMER * timer = al_create_timer(1.0/30.0);
    init(timer, "timer");

    ALLEGRO_EVENT_QUEUE * events = al_create_event_queue();
    init(events,"event queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    ALLEGRO_DISPLAY * display = al_create_display(MAZE_SIZE * CELL_SIZE + 10,MAZE_SIZE * CELL_SIZE + 10);
    init(display,"display");

    al_register_event_source(events, al_get_timer_event_source(timer));
    al_register_event_source(events, al_get_display_event_source(display));

    if (DEBUG) std::cout << "Started generation of maze" << std::endl;
    Maze maze;
    if (DEBUG) std::cout << "Finished generation of maze" << std::endl;

    ALLEGRO_EVENT event;
    bool finish = false;
    bool redraw = true;
    al_start_timer(timer);
    while (true){
        al_wait_for_event(events, &event);
        if (DEBUG) std::cout << "Got the event" << std::endl;
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                finish = true;
                break;
        }

        if (redraw && al_is_event_queue_empty(events)){
            al_clear_to_color(al_map_rgb_f(0,0,0));
            if (DEBUG) std::cout << "Drawing the maze.." << std::endl;
            maze.draw(5,5);
            if (DEBUG) std::cout << "Finished drawing the maze.." << std::endl;

            al_flip_display();
            redraw = false;
        }

        if (finish) break;

    }

    al_destroy_timer(timer);
    al_destroy_display(display);


}
