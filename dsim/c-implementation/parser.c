/* 
 * Parser
 *
 */

#include "types.h"

int sanity_check(struct share *share)
{
  if (share != NULL)
    {
      printf ("Share: percentage %d bdate %d edate %d\n", share->percentage, share->bdate, share->edate);
      sanity_check(share->next);
    }
  else return 0;
}


int share_from_string(struct  share **share, char *line)
{
 
  *share = (struct share *) malloc (sizeof(struct share)); 
 
  char *nxt;
  (*share)->percentage = strtod(line, &nxt); 
  
  (*share)->bdate = strtod(nxt, &nxt);
  (*share)->edate = strtod(nxt, &nxt);
  
  if ((*share)->percentage == 0 || (*share)->bdate == 0 || (*share)->edate == 0)
    {  *share = NULL; return 0; }
  else 
    {
      share_from_string(&((*share)->next), nxt); 
      return 0;
    }
}

int demand_from_string (struct demand **demand, char *line)
{
 
  *demand = (struct demand *) malloc (sizeof(struct demand)); 
  (*demand)->next = NULL; 
  
  int rc; 
  
  (*demand)->origin = (char *) malloc (4 * sizeof(char)); 
  (*demand)->destination = (char *) malloc (4 * sizeof(char)); 
 

  ((*demand)->origin)[0] = line[0]; 
  ((*demand)->origin)[1] = line[1]; 
  ((*demand)->origin)[2] = line[2];
  ((*demand)->origin)[3] = '\0'; 

  ((*demand)->destination)[0] = line[4]; 
  ((*demand)->destination)[1] = line[5]; 
  ((*demand)->destination)[2] = line[6]; 
  ((*demand)->destination)[3] = '\0'; 
  
  char * nxt; 
  (*demand)->bookings_per_day = strtod(&(line[7]), &nxt); 
  
  rc = share_from_string(&((*demand)->repartition), nxt); 

  
  return rc; 
}
  
  
/*
int main ()
{
  struct demand *dmd; 
  char* is = "NCE JFK 10 10 20 30 20 150 170 absa 10 154 "; 
  int rc; 
  rc = demand_from_string(&dmd, is); 
  printf("demand_from_string: %d\n", rc); 
  
  sanity_check(dmd->repartition);
  return 0;
}
*/
