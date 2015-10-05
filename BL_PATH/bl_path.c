/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
int get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy);

// first try at pathfinding
void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord) // should return a list (queue?) of which nodes to take to the end
{
    int x = coord.sx;
    int y = coord.sy;
    int endx = coord.ex;
    int endy = coord.ey;
    int chx = 0, chy = 0;
    int visited_direction;
    int i = 0;
    
    bl_path_pathnode **nodeField = NULL;
    
    nodeField = calloc(fieldx, sizeof(bl_path_pathnode *));
    for(i = 0; i < fieldx; i++) {
        nodeField[i] = calloc(fieldy, sizeof(bl_path_pathnode));
    }
    
    
    while (x != endx || y != endy) {
        //find correct angle to move
        visited_direction = get_chxy_direction(x, y, endx, endy, &chx, &chy);

        if(field[chx][chy].open) { // next node is open
            nodeField[x][y].visited |= 1 << visited_direction;
            nodeField[chx][chy].visited |= 1 << ((visited_direction + 4) % 8);
        } else {
            // TODO: go through options on which way to go
            ;
        }
        
        printf("field[%d][%d].visited = %X\n", x, y, (int)nodeField[x][y].visited);
        // check if next node is open, if yes set appropriate pathnode.visited bit and go there (loop)
        // work on splits when the node is closed
        
        x = chx;
        y = chy;
    }
}

int get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy)
{
    int dir[3][3] = {{5, 6, 7}, {4, -1, 0}, {3, 2, 1}};
    int rise, run;
    int visited_direction;
    float slope;
    
    // should only be one or the other here
    rise = (endy - y);
    run = (endx - x);
    if(run == 0) {
        if(rise > 0) {
            slope = 3.0;
        } else {
            slope = -3.0;
        }
    } else if(rise == 0) {
        if(run > 0) {
            slope = 0.0;
        } else {
            slope = -0.0;
        }
    } else {
        slope = (float)rise / (float)run;
    }
    
    //     angles    |  slope
    // -22.5 -- 22.5 | < 0.414
    // 22.5 -- 67.5 | 0.414 -- 2.414
    // 67.5 -- 90.0 | > 2.414
    // from this get chx and chy from dir[]
    
    if(slope < 0) {
        slope *= -1;
    }
    
    if(slope < 0.414) {
        *chx = 1; *chy = 0;
    } else if(slope > 2.414) {
        *chx = 0; *chy = 1;
    } else {
        *chx = 1; *chy = 1;
    }
    
    if(rise < 0) {
        *chy *= -1;
    }
    if(run < 0) {
        *chx *= -1;
    }
    
    //*chx = (endx - x) > 0 ? 1 : (endx - x) < 0 ? -1 : 0;
    //*chy = (endy - y) > 0 ? 1 : (endy - y) < 0 ? -1 : 0;
    
    visited_direction = dir[*chx + 1][*chy + 1];
    *chx += x;
    *chy += y;
    
    return visited_direction;
}
