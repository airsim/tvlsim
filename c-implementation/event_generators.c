/*
 * C prototype - event generator
 *
 */

#include <stdio.h>
#include <malloc.h>

#include "types.h"

int init_generator(int gen_id, int ncols, FILE *fp, struct generator **cur_gen)
{
  *cur_gen = (struct generator *) malloc (sizeof(struct generator)); 
  (*cur_gen)->counter = 0; 
  (*cur_gen)->type = gen_id; 
 
  int c;
  
  int dtd1, dtd2; 
  int th1, th2; 
  
  fscanf(fp, "%d; %d; %d; %d; ", &dtd1, &th1, &dtd2, &th2);
 
  struct period * cur_seg = (struct period *) malloc (sizeof(struct period)); 
  
  cur_seg->length = dtd2 - dtd1; 
  cur_seg->begin = dtd1; 
  cur_seg->remaining = th1 - th2; 
  cur_seg->next_period = NULL; 
  
  for (c = 0; c < ncols - 3; c++)
    {
      dtd1 = dtd2 ;
      th1 = th2; 
      if (fscanf(fp, "%d; %d; ", &dtd2, &th2) != 2) {return -1;} 
 
      struct period * new_seg = (struct period *) malloc (sizeof(struct period)); 
      new_seg->length = dtd2 - dtd1; 
      new_seg->begin = dtd1; 
      new_seg->remaining = th1 - th2; 
      new_seg->next_period = cur_seg; 
      cur_seg = new_seg; 
    }
  
  dtd1 = dtd2 ;
  th1 = th2;
  if (fscanf(fp, "%d; %d;", &dtd2, &th2) != 2) {return -1;}
  struct period * new_seg = (struct period *) malloc (sizeof(struct period)); 
  new_seg->length = dtd2 - dtd1; 
  new_seg->begin = dtd1;   
  new_seg->remaining = th1 - th2; 
  new_seg->next_period = cur_seg; 
  cur_seg = new_seg; 
 
 
  (*cur_gen)->cur_period = cur_seg; 
 
  return 0;

}

int init_generators(struct generator ***generators, int *ngenerators, const char* arrivalPatterns)
{
  FILE * fp; 
  
  fp = fopen(arrivalPatterns, "r"); 
  if (!fp) {return -1; }
  
  int ncols, nrows; 
  if (fscanf(fp, "%d, %d\n", &nrows, &ncols) != 2) return -1 ;

  *generators = (struct generator **) malloc (ncols * sizeof(int)); 
  *ngenerators = nrows; 
  
  int gen_id; 
  
  for (gen_id = 0; gen_id < nrows; gen_id++)
    {
      if (init_generator(gen_id, ncols, fp, &((*generators)[gen_id])) != 0) return -1; 

    }

  fclose(fp);
  return 0;
}

int get_generator_next_event(struct generator *state, struct event *ev)
{

  if (state->cur_period == NULL) { return -2; }

  struct period *p = state->cur_period; 

  if (state->cur_period->remaining == 0)
    {
      state->cur_period = state->cur_period->next_period; 
      return -1; 
    }
  
  ev->date = rand () % (p->length) + p->begin; 
  state->cur_period->remaining = state->cur_period->remaining - 1; 

  ev->id = state->counter;
  ev->type = state->type;
  ev->next_event = NULL; 
  state->counter++; 
  return 0;
 
}

struct event *insert (struct event *new_event, struct event *head_event_list)
{
  if (head_event_list == NULL) {return new_event; }
  if (new_event->date > head_event_list->date)
    {
      new_event->next_event = head_event_list; 
      return new_event; 
    }

  struct event *nxt = insert (new_event, head_event_list->next_event) ;
  head_event_list->next_event = nxt; 
  return head_event_list; 
}

int get_next_event(struct event_generators *wd, struct event **event)
{
  
  if ((wd->event != NULL))
    {
      *event = wd->event; 
      wd->event = wd->event->next_event; 
      return 0;
    }
  
  int rc; 
  
  struct event *new_event = (struct event *) malloc (sizeof (struct event)); 
  int gen_id; 
  
  for (gen_id = 0; gen_id < wd->ngenerators; gen_id++)
    {
      while ((rc = get_generator_next_event(((wd->generators)[gen_id]), new_event)) == 0)
	{
	  // Insert new_event in the queue
	  
	  wd->event = insert (new_event, wd->event);       
	  
	  new_event = (struct event *) malloc (sizeof (struct event)); 
	  
    } ;
}
  free(new_event); 
  if (rc == -2) return -2; 
  get_next_event (wd, event); 
  return 0;
}


int init_event_generators (struct event_generators *wd, char * arrivalPatterns)
{
  struct generator **generators;
  int ngenerators; 
  
  if (init_generators (&generators, &ngenerators, arrivalPatterns) == -1) { printf ("sth went wrong while parsing the csv file\n"); } 
  
  /*
    int c ; 
  for (c = 0; c < ngenerators; c++)
  {
  printf ("(Sanity check) generator %d out of %d: %s -> %d\n", c, ngenerators, (generators[c])->name, (generators[c])->counter); 
  }
  */
  

  wd->generators = generators ; 
  wd->ngenerators = ngenerators; 
  wd->event = 0;
  return 0;
}


int main(int argc, char **argv)
{
  
  struct event_generators wd; 
  int rc; 

  if ((rc = init_world(&wd, argv[1])) != 0) { return rc; } 
  
  struct event *event01; 

  while (rc = get_next_event (&wd, &event01) == 0)
    {
      printf ("Event type: %d; Event id: %d; Event dtd: %d\n", event01->type, event01->id, event01->date); 
    }
 
  return 0;

}
