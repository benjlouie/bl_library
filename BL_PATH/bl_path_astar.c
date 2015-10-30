/* BL_PATH_ASTAR functions */
/*///////////////////*/

#include "bl_path_astar.h"


void bl_path_arr_astar(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord)
{
    bl_path_pathnode **nodeField = NULL;
    bl_path_list *closed = list_init();
    bl_path_list *open = list_init();
    
    list_add(open, (bl_path_coord) {coord.sx, coord.sy});
    
}
