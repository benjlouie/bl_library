/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"


// first try at pathfinding
void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord) // should return a list (queue?) of which nodes to take to the end
{
    int dir[3][3] = {{5, 6, 7}, {4, -1, 0}, {3, 2, 1}};
    int x = coord.sx;
    int y = coord.sy;
    int endx = coord.ex;
    int endy = coord.ey;
    int chx = 0;
    int chy = 0;
    int visited_direction;
    
    bl_path_pathnode **nodeField = NULL;
    
    nodeField = calloc(fieldx * fieldy, sizeof(bl_path_pathnode)); // might need to malloc each row/column individually

    
    while (x != endx && y != endy) {
        chx = (endx - x) > 0 ? 1 : (endx - x) < 0 ? -1 : 0;
        chy = (endy - y) > 0 ? 1 : (endy - y) < 0 ? -1 : 0;
        
        
        visited_direction = dir[chx + 1][chy + 1];
        nodeField[x][y].visited |= 1 << visited_direction;
        nodeField[x + chx][y + chy].visited |= 1 << ((visited_direction + 4) % 8);
        printf("field[%d][%d].visited = %X", x, y, nodeField[x][y].visited);
        // check if next node is open, if yes set appropriate pathnode.visited bit and go there (loop)
        // work on splits when the node is closed
        
        x += chx;
        y += chy;
    }
}
