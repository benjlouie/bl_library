/* Pathfinding functions */
/*///////////////////*/

#ifndef BL_PATH_H
#define BL_PATH_H

#include <stdio.h>
#include <stdlib.h>

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

typedef struct bl_path_fieldnode_t bl_path_fieldnode;
typedef struct bl_path_pathnode_t bl_path_pathnode;
typedef struct bl_path_sten_t bl_path_sten;



#endif
