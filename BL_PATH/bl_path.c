/* BL_PATH functions */
/*///////////////////*/

#include "bl_path.h"

void bl_path_arr_basic(bl_path_fieldnode **field, int fieldx, int fieldy, bl_path_sten coord);
byte get_chxy_direction(int x, int y, int endx, int endy, int *chx, int *chy);
byte valid_xy(int x, int y, int fieldx, int fieldy);
bl_path_list *list_init(void);
void list_add(bl_path_list *list, bl_path_coord coord);
void list_del(bl_path_list *list, struct bl_path_list_elm *elm);
void list_free(bl_path_list *list);
void list_free_nodes(bl_path_list *list);
void print_pathnode(bl_path_fieldnode **field, bl_path_pathnode **nodeField, int fieldx, int fieldy);
void print_list(bl_path_list *list);

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
    int split_offset[7] = {9, 7, 10, 6, 11, 5, 12};
    int x = coord.sx;
    int y = coord.sy;
    int endx = coord.ex;
    int endy = coord.ey;
    int chx = 0, chy = 0;
    byte visited_direction;
    byte tmp_visited = 0;
    bl_path_pathnode **nodeField = NULL;
    bl_path_list *list = NULL;
    struct bl_path_list_elm *cur = NULL;
    struct bl_path_list_elm *tmp = NULL;
    int i = 0;
    
    // initialization
    nodeField = malloc(fieldx * sizeof(bl_path_pathnode *));
    for(i = 0; i < fieldx; i++) {
        nodeField[i] = calloc(fieldy, sizeof(bl_path_pathnode));
    }
    list = list_init();
    list_add(list, (bl_path_coord) {x, y});
    nodeField[x][y].visited = 1; //TODO: should be zero, that will indicate starting node
    
    //TODO: delete this debug printing
    print_pathnode(field, nodeField, fieldx, fieldy);
    
    cur = list->head;
    
    while (list->head != NULL) {
        if(cur == NULL)
            cur = list->head;

        x = cur->coord.x;
        y = cur->coord.y;
        if(x == endx && y == endy) {
            // TODO: path found, cleanup the rest of the list
            break;
        }
        
        // go the most direct way
        // if unavailable, split into the 4 surrounding possible directions
        // add each split direction to the list
        visited_direction = get_chxy_direction(x, y, endx, endy, &chx, &chy);
        nodeField[x][y].split |= 1 << visited_direction;
        if(valid_xy(x, y, fieldx, fieldy)) {
            // TODO: put in stopping case for out of bounds chx,chy moves
            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) { // next node is open and unvisited
                nodeField[chx][chy].visited = 1 << ((visited_direction + 4) % 8);
            } else {
                // go in alternate directions
                for(i = 0; i < 7; i++) {
                    tmp_visited = (visited_direction + split_offset[i]) % 8; // 8 possible split directions

                    if(!(nodeField[x][y].split & (1 << tmp_visited))) {
                        // not visited, create new split
                        chx = x + dirx[tmp_visited];
                        chy = y + diry[tmp_visited];
                        if(valid_xy(chx, chy, fieldx, fieldy)) {
                            // TODO: check this, not including it should stop pile up
                            if(field[chx][chy].open && !(nodeField[chx][chy].visited)) {
                                nodeField[x][y].split |= (1 << tmp_visited);
                                nodeField[chx][chy].visited = 1 << ((tmp_visited + 4) % 8); //TODO: check if "=" is ok instead of "|="
                                list_add(list, (bl_path_coord) {chx, chy});
                                if(!FIND_OPTIMAL)
                                    break; // finds first path, not always optimal (much less node expansion)
                            }
                        }
                    }
                }

                tmp = cur->next;
                list_del(list, cur);
                cur = tmp;
                continue;
            }

            x = chx;
            y = chy;
            cur->coord = (bl_path_coord) {x, y};
            cur = cur->next;
        } else {
            tmp = cur->next;
            list_del(list, cur);
            cur = tmp;
        }
    }
    
    list_free_nodes(list);
    list_add(list, (bl_path_coord) {endx, endy});
    x = coord.sx; y = coord.sy;
    chx = endx; chy = endy;
    while(chx != x || chy != y) {
        tmp_visited = nodeField[chx][chy].visited;
        for(i = 0; !(tmp_visited & 0x1); i++) { // find direction of previous node
            tmp_visited >>= 1;
        }
        chx = chx + dirx[i];
        chy = chy + diry[i];
        list_add(list, (bl_path_coord) {chx, chy});
    }
    print_list(list);
    
    for(x = 0; x < fieldx; x++) {
        for(y = 0; y < fieldy; y++) {
            nodeField[x][y].visited = 0;
        }
    }
    cur = list->head;
    while(cur != NULL) {
        nodeField[cur->coord.x][cur->coord.y].visited = 255;
        cur = cur->next;
    }
    
    
    //TODO: delete this debug printing
    print_pathnode(field, nodeField, fieldx, fieldy);
    
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

byte valid_xy(int x, int y, int fieldx, int fieldy)
{
    return !((x >= fieldx || x < 0) || (y >= fieldy || y < 0));
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

void list_del(bl_path_list *list, struct bl_path_list_elm *elm)
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

void list_free_nodes(bl_path_list *list)
{
    struct bl_path_list_elm *cur = NULL, *next = NULL;;
    cur = list->head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
}

void print_pathnode(bl_path_fieldnode **field, bl_path_pathnode **nodeField, int fieldx, int fieldy)
{
    int x, y;
    
    
    for(y = fieldy - 1; y >= 0; y--) {
        printf("%3d ", y);
        for(x = 0; x < fieldx; x++) {
            if(field[x][y].open == 0)
                printf("%c%c%c", 178, 178, 178);
            else if(nodeField[x][y].visited)
                printf(" # ");
            else
                printf("%c%c%c", 176, 176, 176);
        }
        printf("\n");
    }
    // print
    printf("   ");
    for(x = 0; x < fieldx; x++) {
        printf("%3d", x);
    }
    printf("\n");
}

void print_list(bl_path_list *list)
{
    struct bl_path_list_elm *cur = list->head;
    
    while(cur != NULL) {
        printf("(%d,%d) ", cur->coord.x, cur->coord.y);
        cur = cur->next;
    }
    printf("\n");
}
