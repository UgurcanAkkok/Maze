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
    void adjust(ALLEGRO_VERTEX * vertices,int vertex_count, int d){
        for (int i = 0; i < vertex_count; i++){
            if ((vertices + i)->x >= (WIDTH / 2)){
                (vertices + i)->x -= d;
            }
            else {
                (vertices + i)->x += d;
            }

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

};

class Wall : protected Object {
    public:
    Wall (int x, int y, int w, int h, int d){
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

    public:
        void draw();
        void map_view_to_graphics();

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
            map_view_to_graphics();
            draw();
        }

        ~Environment(){
            delete [] view;
        }
    
};
