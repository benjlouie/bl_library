/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
byte get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy);

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
    byte visited_direction;
    bl_path_pathnode **nodeField = NULL;
    bl_path_que que;
    int i = 0;
    
    nodeField = malloc(fieldx * sizeof(bl_path_pathnode *));
    for(i = 0; i < fieldx; i++) {
        nodeField[i] = calloc(fieldy, sizeof(bl_path_pathnode));
    }
    que_init(&que, 16); // TODO: change to calculated value of reasonable size
    
    
    // TODO: add in que system
    while (x != endx || y != endy) {
        //find correct angle to move
        visited_direction = get_chxy_direction(x, y, endx, endy, &chx, &chy);

        if(field[chx][chy].open && !(nodeField[chx][chy].visited)) { // next node is open and unvisited
            nodeField[x][y].visited |= 1 << visited_direction;
            nodeField[chx][chy].visited |= 1 << ((visited_direction + 4) % 8);
        } else {
            nodeField[x][y].visited |= 1 << visited_direction;
            
            // TODO: get alternate directions it could go, if it can only go backward, then go to the previous node and split to an alt direction
            // only go +-2 to visited direction
            for(i = 1; i <= 2; i++) {
                // TODO: add both directions to que here
            }
            
            //visited_direction = (visited_direction + 9) % 8;
            
        }
        
        printf("field[%d][%d].visited = "BYTETOBINARYPATTERN"\n", x, y, BYTETOBINARY(nodeField[x][y].visited));
        
        x = chx;
        y = chy;
    }
    
    free(nodeField);
    free(que);
}

byte get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy)
{
    byte dir[3][3] = {{5, 6, 7}, {4, -1, 0}, {3, 2, 1}};
    int rise, run;
    byte visited_direction;
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


////// TODO: change que system to pointers (so nodes in the middle can be deleted) //////
void que_init(bl_path_que *que, int init_spaces)
{
    que->que = malloc(sizeof(bl_path_coord) * init_spaces);
    que->size = init_spaces;
    que->index = 0;
}

void que_add(bl_path_que *que, bl_path_coord coord)
{
    if(que->index >= que->size) {
        que->que = realloc(que->que, (que->size *= 2) * sizeof(bl_path_coord));
    }
    que->que[que->index++] = coord;
}

bl_path_coord que_del(bl_path_que *que)
{
    if(que->index > 0) {
        que->index--;
        return que->que[que->index + 1];
    }
    bl_path_coord bad = {-1, -1};
    return bad;
}

void que_free(bl_path_que *que)
{
    free(que->que);
}
