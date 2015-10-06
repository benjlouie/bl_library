#include <stdio.h>
#include "bl_path.h"


int main(void)
{

    int i,j;
    int fieldx = 10;
    int fieldy = 10;
    bl_path_fieldnode **field = NULL;
    bl_path_sten coords;
    
    // init
    field = malloc(sizeof(bl_path_fieldnode *) * fieldx);
    for(i = 0; i < fieldx; i++) {
        field[i] = malloc(sizeof(bl_path_fieldnode) * fieldy);
    }
    
    for(i = 0; i < fieldx; i++) {
        for(j = 0; j < fieldy; j++) {
            field[i][j].open = 1;
        }
    }
    coords.sx = 1;
    coords.sy = 1;
    coords.ex = 8;
    coords.ey = 8;

    bl_path_arr_basic(field, fieldx, fieldy, coords);

	return 0;
}
