#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "maze_gen.cpp"

#define WIDTH 1280
#define HEIGHT 720

class Object {
    protected:
    ALLEGRO_VERTEX * vertices;
    ALLEGRO_COLOR obj_color;

    public:
    void adjust(ALLEGRO_VERTEX * vertices,int vertex_count,int d){
        int base_x = 0;
        int base_y = 0;
        int base_d = d;
        for (int i = 0; i < vertex_count; i++){
            base_x += vertices[i].x;
        }
        base_x /= vertex_count;
        for (int i = 0; i < vertex_count; i++){
            base_y += vertices[i].y;
        }
        base_y /= vertex_count;
        for (int i = 0; i < vertex_count; i++){
            d = vertices[i].x - base_d;
            if ((vertices + i)->x >= (WIDTH / 2)){
                (vertices + i)->x -= d;
            }
            else {
                (vertices + i)->x += d;
            }
            
            d = vertices[i].y - base_d;
            if ((vertices + i)->y >= (HEIGHT / 2)){
                (vertices + i)->y -= d;
            }
            else {
                (vertices + i)->y += d;
            }
        }
    }
};

class Floor : protected Object {
    public:
        Floor(ALLEGRO_COLOR color = al_map_rgb(51, 10, 0)){
            vertices = new ALLEGRO_VERTEX [4];
            obj_color = color;
            vertices[0].x = 0;
            vertices[0].y = 0;
            vertices[0].color = obj_color;
            
            vertices[1].x = WIDTH;
            vertices[1].y = 0;
            vertices[1].color = obj_color;

            vertices[2].x = WIDTH;
            vertices[2].y = HEIGHT;
            vertices[2].color = obj_color;

            vertices[3].x = 0;
            vertices[3].y = HEIGHT;
            vertices[3].color = obj_color;
        }
        
        void draw(){
            al_draw_prim(vertices, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
        }

        ~Floor(){
            delete [] vertices;
        }
};

class Wall : protected Object {
    public:
    Wall (int x, int y, int d, int w = 100, int h = 100){
        vertices = new ALLEGRO_VERTEX [4];
        obj_color = al_map_rgb(230, 46, 0);

        vertices[0].x = x;
        vertices[0].y = y;
        vertices[0].color = obj_color;
        
        vertices[1].x = x + w;
        vertices[1].y = y;
        vertices[1].color = obj_color;

        vertices[2].x = x + w;
        vertices[2].y = y + h;
        vertices[2].color = obj_color;

        vertices[3].x = x;
        vertices[3].y = y + h;
        vertices[3].color = obj_color;

        adjust(vertices, 4, d);

        }

    void draw(){
        al_draw_prim(vertices, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
        for (int i = 0; i < 4; i++){
            vertices[i].color = al_map_rgb(0,0,0);
        }
        al_draw_prim(vertices, NULL, NULL, 0, 3, ALLEGRO_PRIM_LINE_LIST);
    }
    ~Wall (){
        delete [] vertices;
    }

};

class Environment {
    private:
        int x,y;
        directions d;
        Cell ** view;
        Floor ground;

    public:
        void draw(){
            ground.draw();
            
        }
        void create_view(Maze * maze){
            int corridor_length = 0;
            while (true) {
                bool front_open = false;
                int view_x = x, view_y = y;
                switch (d) {
                    case right:
                        if (maze->maze_map[view_x][view_y].right_open)
                            front_open = true;
                            view_x +=1;
                            corridor_length++;
                        break;
                    case left:
                        if (maze->maze_map[view_x][view_y].left_open)
                            front_open = true;
                            view_x -=1;
                            corridor_length++;
                        break;
                    case up:
                        if (maze->maze_map[view_x][view_y].up_open)
                            front_open = true;
                            view_y -=1;
                            corridor_length++;
                        break;
                    case down:
                        if (maze->maze_map[view_x][view_y].down_open)
                            front_open = true;
                            view_y +=1;
                            corridor_length++;
                        break;
                }
                if (!front_open) break;
            }
            view = new Cell * [corridor_length];
            Cell view_cell = maze->maze_map[x][y];
            for (int i = 0; i < corridor_length; i++){
                switch(d){
                    case right:
                        view[i][0] = *maze->adjacent(view_cell.x, view_cell.y, up);
                        break;
                    case left:
                        view[i][0] = *maze->adjacent(view_cell.x, view_cell.y, down);
                        break;
                    case down:
                        view[i][0] = *maze->adjacent(view_cell.x, view_cell.y, right);
                        break;
                    case up:
                        view[i][0] = *maze->adjacent(view_cell.x, view_cell.y, left);
                        break;
                }
                view[i][1] = view_cell;

                switch(d){
                    case right:
                        view[i][2] = *maze->adjacent(view_cell.x, view_cell.y, down);
                        break;
                    case left:
                        view[i][2] = *maze->adjacent(view_cell.x, view_cell.y, up);
                        break;
                    case down:
                        view[i][2] = *maze->adjacent(view_cell.x, view_cell.y, left);
                        break;
                    case up:
                        view[i][2] = *maze->adjacent(view_cell.x, view_cell.y, right);
                        break;
                }
                view_cell = *maze->adjacent(view_cell.x, view_cell.y, d);
            }

        }

        Environment(Maze * maze, int x, int y, directions d) : x(x), y(y), d(d) {
            create_view(maze);
            draw();
        }

        ~Environment(){
            delete [] view;
        }
    
};
