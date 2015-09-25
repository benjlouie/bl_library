/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

struct bl_path_fieldnode_t {
    char open; // open or not
    char data1; // future data
    char data2; // future data
};

struct bl_path_pathnode_t {
    char visited;
    char split; // notifies splits and merges in path
    //int count; // the number of moves since the start to that node (for easier queue creation)
};

struct bl_path_sten_t {
    int sx; //start x
    int sy; //start y
    int ex; //end x
    int ey; //end y
};



// first try at pathfinding
void bl_path_arr_basic(bl_part_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord) // should return a list (queue?) of which nodes to take to the end
{
    int x = coord.sx;
    int y = coord.sy;
    int endx = coord.ex;
    int endy = coord.ey;
    int chx = 0;
    int chy = 0;
    
    bl_path_pathnode **nodeField = NULL;
    
    nodeField = calloc(fieldx * fieldy, sizeof(bl_path_pathnode)); // might need to malloc each row/column individually

    
    while (x != endx && y != endy) {
        chx = (endx - x) > 0 ? 1 : (endx - x) < 0 ? -1 : 0;
        chy = (endy - y) > 0 ? 1 : (endy - y) < 0 ? -1 : 0;
        
        // check if next node is open, if yes set appropriate pathnode.visited bit and go there (loop)
        // work on splits when the node is closed
    }
}
