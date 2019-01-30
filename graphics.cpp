#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "maze_gen.cpp"
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
                view_cell = *maze->adjacent(view_cell.x, view_cell.y, d);
            }

        }
        Environment(Maze * maze, int x, int y, directions d) : x(x), y(y), d(d) {
            create_view(maze);
            map_view_to_graphics();
            draw();
        }
    
};
