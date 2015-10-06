/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
int get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy);

//  TODO: Delete this (debug stuff)
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 


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
    
    nodeField = malloc(fieldx * sizeof(bl_path_pathnode *));
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
            nodeField[x][y].visited |= 1 << visited_direction;
            //nodeField[x][y].split |= ~(nodeField[x][y].visited); // options of where it can go
            
            // TODO: get alternate directions it could go, if it can only go backward, then go to the previous node and split to an alt direction
            //visited_direction = (visited_direction + 9) % 8;
            
        }
        
        printf("field[%d][%d].visited = "BYTETOBINARYPATTERN"\n", x, y, BYTETOBINARY(nodeField[x][y].visited));
        
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
        slope = 3.0;
    } else if(rise == 0) {
        slope = 0.0;
    } else {
        slope = (float)rise / (float)run;
    }
    
    if(slope < 0) {
        slope *= -1;
    }
    
    if(slope < 0.414) { // change in x only
        *chx = 1; *chy = 0;
    } else if(slope > 2.414) { // change in y only
        *chx = 0; *chy = 1;
    } else { // change in x and y
        *chx = 1; *chy = 1;
    }
    
    if(run < 0) {
        *chx *= -1;
    }
    if(rise < 0) {
        *chy *= -1;
    }
    
    visited_direction = dir[*chx + 1][*chy + 1];
    *chx += x;
    *chy += y;
    
    return visited_direction;
}
