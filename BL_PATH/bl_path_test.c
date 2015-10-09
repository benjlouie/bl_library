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
    
    field[5][5].open = 0;
    field[5][4].open = 0;
    coords = (bl_path_sten) {1, 5, 8, 5};

    bl_path_arr_basic(field, fieldx, fieldy, coords);

	return 0;
}
