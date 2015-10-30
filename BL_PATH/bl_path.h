/* Pathfinding functions */
/*///////////////////*/

#ifndef BL_PATH_H
#define BL_PATH_H

#include <stdio.h>
#include <stdlib.h>

#define FIND_OPTIMAL 1 // if yes(1), finds the optimal path. if no(0), finds a <= optimal path (faster)

typedef struct bl_path_fieldnode_t bl_path_fieldnode;
typedef struct bl_path_pathnode_t bl_path_pathnode;
typedef struct bl_path_sten_t bl_path_sten;
typedef struct bl_path_list_t bl_path_list;
typedef struct bl_path_coord_t bl_path_coord;

typedef unsigned char byte;


struct bl_path_fieldnode_t {
    byte open; // open or not
    byte data1; // future data
    byte data2; // future data
};

struct bl_path_pathnode_t {
    byte visited;
    byte split; // notifies splits and merges in path
    //int count; // the number of moves since the start to that node (for easier queue creation)
};

struct bl_path_sten_t {
    int sx; //start x
    int sy; //start y
    int ex; //end x
    int ey; //end y
};

struct bl_path_coord_t {
    int x;
    int y;
};

struct bl_path_list_t {
     struct bl_path_list_elm {
         bl_path_coord coord;
         struct bl_path_list_elm *next;
         struct bl_path_list_elm *prev;
     } *head;
     int size;
};


void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
bl_path_list *list_init(void);
void list_add(bl_path_list *list, bl_path_coord coord);
void list_del(bl_path_list *list, struct bl_path_list_elm *elm);
void list_free(bl_path_list *list);
void list_free_nodes(bl_path_list *list);
void print_pathnode(bl_path_fieldnode **field, bl_path_pathnode **nodeField, int fieldx, int fieldy);
void print_list(bl_path_list *list);

#endif
