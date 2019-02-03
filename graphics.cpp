#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>

#include "maze_gen.cpp"

#define WIDTH 1280
#define HEIGHT 720

struct MAZE_VERTEX {
    int x,y,z;
    ALLEGRO_COLOR color;
    ALLEGRO_VERTEX to_al_vertex(){
        ALLEGRO_VERTEX al_vertex;
        al_vertex.x = this->x;
        al_vertex.y = this->y;
        al_vertex.color = this->color;
        return al_vertex;
    }
       void rotate_z(MAZE_VERTEX center, double angle_in_degrees){
           double angle_in_rad = angle_in_degrees * (ALLEGRO_PI / 180);
           double cos_theta = cos(angle_in_rad);
           double sin_theta = sin(angle_in_rad);
           int point_x = this->x;
           int point_y = this->y;
           this->x = (int)(cos_theta * (point_x - center.x) - 
                   sin_theta * (point_y - center.y) + center.x);
           this->y = (int)(sin_theta * (point_x - center.x) +
                   cos_theta * (point_y - center.y) + center.y);
       }
       void rotate_x(MAZE_VERTEX center, double angle_in_degrees){
           double angle_in_rad = angle_in_degrees * (ALLEGRO_PI / 180);
           double cos_theta = cos(angle_in_rad);
           double sin_theta = sin(angle_in_rad);
           int point_z = this->z;
           int point_y = this->y;
           this->z = (int)(cos_theta * (point_z - center.z) - 
                   sin_theta * (point_y - center.y) + center.z);
           this->y = (int)(sin_theta * (point_z - center.z) +
                   cos_theta * (point_y - center.y) + center.y);
       }
       void rotate_y(MAZE_VERTEX center, double angle_in_degrees){
           double angle_in_rad = angle_in_degrees * (ALLEGRO_PI / 180);
           double cos_theta = cos(angle_in_rad);
           double sin_theta = sin(angle_in_rad);
           int point_z = this->z;
           int point_x = this->x;
           this->z = (int)(cos_theta * (point_z - center.z) - 
                   sin_theta * (point_x - center.x) + center.z);
           this->x = (int)(sin_theta * (point_z - center.x) +
                   cos_theta * (point_x - center.x) + center.x);
       }
       void move_z(int d){
           this->z += d;
       }
       void move_x(int d){
           this->x += d;
       }
       void move_y(int d){
           this->y += d;
       }

};
class Plane {
    int w, h;

    public:
        MAZE_VERTEX vertices [4];
        MAZE_VERTEX rotate_center;
        Plane (int x0, int y0, int w, int h) : w(w), h(h) {
            vertices[0].x = x0;
            vertices[0].y = y0;
            MAZE_VERTEX rotate_center;
            int avg_x = 0, avg_z = 0, avg_y = 0;
            for (int i = 0; i < 4; i++){
                avg_x += vertices[i].x;
                avg_y += vertices[i].y;
                avg_z += vertices[i].z;
            }
            avg_x /= 4;
            avg_y /= 4;
            avg_z /= 4;
            rotate_center.x = avg_x;
            rotate_center.y = avg_y;
            rotate_center.z = avg_z;

        }
        void fill_color(ALLEGRO_COLOR color){
            for (int i = 0; i < 4; i++){
                vertices[i].color = color;
            }
        }
        void rotate_z (int angle){
            for (int i = 0; i < 4; i++){
                vertices[i].rotate_z(rotate_center, angle);
            }
        }
        void rotate_y (int angle){
            for (int i = 0; i < 4; i++){
                vertices[i].rotate_y(rotate_center, angle);
            }
        }
        void rotate_x (int angle){
            for (int i = 0; i < 4; i++){
                vertices[i].rotate_x(rotate_center, angle);
            }
        }
        void move_x (int d){
            for (int i = 0; i < 4; i++){
                vertices[i].move_x(d);
            }
        }
        void move_y (int d){
            for (int i = 0; i < 4; i++){
                vertices[i].move_y(d);
            }
        }
        void move_z (int d){
            for (int i = 0; i < 4; i++){
                vertices[i].move_z(d);
            }
        }
        
};

class Environment {
    private:
        int x,y;
        directions d;
        Cell ** view;

    public:
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
        }

        ~Environment(){
            delete [] view;
        }
    
};
