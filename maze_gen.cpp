#include <cmath>
#include <ctime>

#define MAZE_SIZE 20
#define CELL_SIZE 5

class Maze;
class Cell;

class Cell {
    public:
        bool left_edge = false;
        bool right_edge = false;
        bool up_edge = false;
        bool down_edge = false;

        bool visited = false;

        bool left_open = false;
        bool right_open = false;
        bool up_open = false;
        bool down_open = false;
        
        void init(int x, int y){
            if (x == MAZE_SIZE)
                right_edge = true;
            if (x == 0)
                left_edge = true;
            if (y == MAZE_SIZE)
                down_edge = true;
            if (y == 0)
                up_edge = true;
        }
        void draw(int,int);
};

class Maze {
    private:
        int start_x;
        int start_y;
        Cell maze_map[MAZE_SIZE][MAZE_SIZE];
    public:

        void rand_gen(int x,int y){
            maze_map[x][y].visited = true;
            bool correct_way = false;
            int direction;
            if (maze_map[x - 1][y].visited && maze_map[x][y - 1].visited
                    && maze_map[x + 1][y].visited && maze_map[x][y + 1].visited)
                return;

            while (!correct_way){
                direction = rand() % 4 + 1; // 1:left 2:down 3:right 4:up
                switch(direction){
                    case 1:
                        if (maze_map[x][y].left_edge == true){
                            correct_way = false;
                            break;
                        }
                        if (maze_map[x - 1][y].visited == false ){
                            correct_way = true;
                            maze_map[x][y].left_open = true;
                            maze_map[x - 1][y].right_open = true;
                        }
                        else {
                            correct_way = false;
                        }

                        if (correct_way){
                            rand_gen(x - 1, y);
                        }
                        break;

                    case 2:
                        if (maze_map[x][y].down_edge == true){
                            correct_way = false;
                            break;
                        }
                        if (maze_map[x][y + 1].visited == false && maze_map[x][y].down_edge != true){
                            correct_way = true;
                            maze_map[x][y].down_open = true;
                            maze_map[x][y + 1].up_open = true;
                        }
                        else {
                            correct_way = false;
                        }
                        if (correct_way){
                            rand_gen(x, y + 1);
                        }
                        break;
                    case 3:
                        if (maze_map[x][y].right_edge == true){
                            correct_way = false;
                            break;
                        }
                        if (maze_map[x + 1][y].visited == false && maze_map[x][y].right_edge != true){
                            correct_way = true;
                            maze_map[x][y].right_open = true;
                            maze_map[x + 1][y].left_open = true;
                        }
                        else {
                            correct_way = false;
                        }
                        if (correct_way){
                            rand_gen(x + 1, y);
                        }
                        break;
                    case 4:
                        if (maze_map[x][y].up_edge == true){
                            correct_way = false;
                            break;
                        }
                        if (maze_map[x][y - 1].visited == false && maze_map[x][y].up_edge != true){
                            correct_way = true;
                            maze_map[x][y].up_open = true;
                            maze_map[x][y - 1].down_open = true;
                        }
                        else {
                            correct_way = false;
                        }
                        if (correct_way){
                            rand_gen(x, y - 1);
                        }
                        break;
                }
            
            }

        }

        Maze(){
            for (int i = 0; i < MAZE_SIZE; i++){
                for (int j = 0; j < MAZE_SIZE; j++){
                    maze_map[i][j].init(i,j);
                }
            }
            srand(time(NULL));
            start_x = rand() % MAZE_SIZE;            
            start_y = rand() % MAZE_SIZE;            
            rand_gen(start_x,start_y);

        }

        void draw(int,int);
};
