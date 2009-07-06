/*
 * dSim prototype
 *
 */

// O-D -> int translator; its a map

#include <stdlib.h>
#include <stdio.h>





int insert(struct ond **cur_ond_map, char *origin, char *destination, int *counter);

int find(struct ond *cur_ond_map, char *origin, char *destination, int *id);
