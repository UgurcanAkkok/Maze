#define MAZE_SIZE 20
class Maze;
class Cell;

class Cell {
    private:
        bool left_edge = false;
        bool right_edge = false;
        bool up_edge = false;
        bool down_edge = false;
    public:
        bool visited = false;

        bool left_open = false;
        bool right_open = false;
        bool up_open = false;
        bool down_open = false;
        
        Cell(int x, int y){
            if (x == MAZE_SIZE)
                right_edge = true;
            if (x == 0)
                left_edge = true;
            if (y == MAZE_SIZE)
                down_edge = true;
            if (y == 0)
                up_edge = true;
        }
};

class Maze {
    private:
    public:
        Maze(){
            Cell ** maze_map = nullptr;
            for (int i = 0; i < MAZE_SIZE; i++){
                for (int j = 0; j < MAZE_SIZE; j++){
                    *((maze_map + i) + j) = new Cell (i,j);
                }
            }
            *((maze_map + 1) + 2)->visited = true;
        }


};
