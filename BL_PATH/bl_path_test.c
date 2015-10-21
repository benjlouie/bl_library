#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_path.h"


int main(void)
{

    int i,j;
    int fieldx = 24;
    int fieldy = 17;
    bl_path_fieldnode **field = NULL;
    bl_path_sten coords;
    int startx = 0;
    int starty = 0;
    int endx = 0;
    int endy = 0;
    
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
    
    srand(time(NULL));
    for(i = 0; i < (fieldx * fieldy) / 2; i++) {
        field[rand() % fieldx][rand() % fieldy].open = 0;
    }
    
    startx = rand() % fieldx;
    starty = rand() % fieldy;
    endx = rand() % fieldx;
    endy = rand() % fieldy;
    
    field[startx][starty].open = 1;
    field[endx][endy].open = 1;    
    
    coords = (bl_path_sten) {startx, starty, endx, endy};
    printf("\n");
    printf("start = %d,%d\nend   = %d,%d\n", startx, starty, endx, endy);
    bl_path_arr_basic(field, fieldx, fieldy, coords);
    printf("start = %d,%d\nend   = %d,%d\n", startx, starty, endx, endy);

	return 0;
}
