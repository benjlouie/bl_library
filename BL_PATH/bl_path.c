/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
byte get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy);
bl_path_list *list_init(void);
void list_add(bl_path_list *list, bl_path_coord coord);
bl_path_coord list_del(bl_path_list *list, struct bl_path_list_elm *elm);
void list_free(bl_path_list *list);

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
    int dirx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int diry[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int x = coord.sx;
    int y = coord.sy;
    int endx = coord.ex;
    int endy = coord.ey;
    int chx = 0, chy = 0;
    byte visited_direction;
    byte tmp_visited = 0;
    bl_path_pathnode **nodeField = NULL;
    bl_path_list *list = NULL;
    int i = 0;
    
    // initialization
    nodeField = malloc(fieldx * sizeof(bl_path_pathnode *));
    for(i = 0; i < fieldx; i++) {
        nodeField[i] = calloc(fieldy, sizeof(bl_path_pathnode));
    }
    list = list_init();
    list_add(list, (bl_path_coord) {x, y});
    
    struct bl_path_list_elm *cur = list->head;
    while (list->head != NULL) {
        if(cur == NULL)
            cur = list->head;
            
        x = cur->coord.x;
        y = cur->coord.y;
        if(x == endx && y == endy) {
            // TODO: path found, cleanup the rest of the list
            break;
        }
        
        visited_direction = get_chxy_direction(x, y, endx, endy, &chx, &chy);
        nodeField[x][y].visited |= 1 << visited_direction;
        if(field[chx][chy].open && !(nodeField[chx][chy].visited)) { // next node is open and unvisited
            nodeField[chx][chy].visited |= 1 << ((visited_direction + 4) % 8);
        } else {
            list_del(list, list->head);
            // go in alternate directions
            
            ////////////// FIX THIS SECTION ///////////////
            tmp_visited = (visited_direction + 9) % 8; // +1 visited
            chx = x + dirx[tmp_visited];
            chy = y + diry[tmp_visited];
            nodeField[x][y].visited |= 1 << tmp_visited;
            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) {
                nodeField[x][y].split |= 1 << tmp_visited;
                nodeField[chx][chy].visited |= 1 << ((tmp_visited + 4) % 8);
                list_add(list, (bl_path_coord) {chx, chy});
            }
            
            tmp_visited = (visited_direction + 7) % 8; // -1 visited
            chx = x + dirx[tmp_visited];
            chy = y + diry[tmp_visited];
            nodeField[x][y].visited |= 1 << tmp_visited;
            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) {
                nodeField[x][y].split |= 1 << tmp_visited;
                nodeField[chx][chy].visited |= 1 << ((tmp_visited + 4) % 8);
                list_add(list, (bl_path_coord) {chx, chy});
            }
            
            tmp_visited = (visited_direction + 10) % 8; // +2 visited
            chx = x + dirx[tmp_visited];
            chy = y + diry[tmp_visited];
            nodeField[x][y].visited |= 1 << tmp_visited;
            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) {
                nodeField[x][y].split |= 1 << tmp_visited;
                nodeField[chx][chy].visited |= 1 << ((tmp_visited + 4) % 8);
                list_add(list, (bl_path_coord) {chx, chy});
            }
            
            tmp_visited = (visited_direction + 6) % 8; // -2 visited
            chx = x + dirx[tmp_visited];
            chy = y + diry[tmp_visited];
            nodeField[x][y].visited |= 1 << tmp_visited;
            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) {
                nodeField[x][y].split |= 1 << tmp_visited;
                nodeField[chx][chy].visited |= 1 << ((tmp_visited + 4) % 8);
                list_add(list, (bl_path_coord) {chx, chy});
            }
            ////////////////////////////////////////////////////
            
        }
        
        printf("field[%d][%d].visited = "BYTETOBINARYPATTERN"\n", chx, chy, BYTETOBINARY(nodeField[chx][chy].visited));
        
        x = chx;
        y = chy;
        cur->coord = (bl_path_coord) {x, y};
        cur = cur->next;
    }
    
    free(nodeField);
    list_free(list);
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
bl_path_list *list_init(void)
{
    bl_path_list *list = malloc(sizeof(bl_path_list));
    list->head = NULL;
    list->size = 0;
    return list;
}

void list_add(bl_path_list *list, bl_path_coord coord)
{
    struct bl_path_list_elm *tmp = NULL;
    tmp = malloc(sizeof(struct bl_path_list_elm));
    tmp->next = list->head;
    tmp->prev = NULL;
    if(list->head != NULL)
        list->head->prev = tmp;
    list->head = tmp;
    tmp->coord = coord;
}

bl_path_coord list_del(bl_path_list *list, struct bl_path_list_elm *elm)
{
    if(elm->prev == NULL) { // head node
        list->head = elm->next;
    } else {
        elm->prev->next = elm->next;
    }
    if(elm->next != NULL) {
        elm->next->prev = elm->prev;
    }
    free(elm);
}

void list_free(bl_path_list *list)
{
    struct bl_path_list_elm *cur = NULL, *next = NULL;;
    cur = list->head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}
