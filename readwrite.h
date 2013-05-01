#include <stdio.h>
#include "universe.h"

void read_life105(FILE *f, 
                  void (*cb)(void *, char, int, int, int, char), 
                  void (*paramcb)(void *, const char *, int),
                  void *cbdata);

void write_life105(FILE *f, generation *g);
void write_life105_text(FILE *f, generation *g);

universe *read_text(const char *filename);

void write_gif(universe *u, int gen, const char *filename);
void write_animated_gif(universe *u, const char *filename);
