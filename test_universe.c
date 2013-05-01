#include <stdlib.h>
#include <assert.h>
#include "universe.h"
#include "readwrite.h"

static void test_universe_basic(void) {
        universe *u;

        u = universe_new(UNKNOWN);
        assert(u);

        universe_evolve_next(u);

        tile *t, *t2;

        t = universe_find_tile(u, 0, 10, 10, 0);
        assert(!t);

        t = universe_find_tile(u, 0, 10, 10, 1);
        assert(t);
        assert(t->xpos == 0);
        assert(t->ypos == 0);

        cellvalue v = tile_get_cell(t, 10, 10);
        assert(v == UNKNOWN);

        tile_set_cell(t, 10, 10, OFF);
        v = tile_get_cell(t, 10, 10);
        assert(v == OFF);

        t2 = universe_find_tile(u, 1, 10, 10, 1);
        assert(t2);
        assert(t2->prev == t);
        assert(t->next == t2);

        t2 = universe_find_tile(u, 0, 10 + TILE_WIDTH, 10, 1);
        assert(t2);
        assert(t2->left == t);
        assert(t->right == t2);

        t2 = universe_find_tile(u, 0, 10, 10 + TILE_HEIGHT, 1);
        assert(t2);
        assert(t2->up == t);
        assert(t->down == t2);

        universe_free(u);
}

static void test_universe_evolve(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;

        t = universe_find_tile(u, 0, 0, 0, 1);
        // place a block
        tile_set_cell(t, 10, 10, ON);
        tile_set_cell(t, 10, 11, ON);
        tile_set_cell(t, 11, 10, ON);
        tile_set_cell(t, 11, 11, ON);

        // and a blinker
        tile_set_cell(t, 20, 20, ON);
        tile_set_cell(t, 21, 20, ON);
        tile_set_cell(t, 22, 20, ON);

        universe_evolve_next(u);
        
        t2 = universe_find_tile(u, 1, 0, 0, 0);
        assert(t2);
        assert(t2->prev == t);

        // verify the block
        assert(tile_get_cell(t2,  9,  9) == OFF);
        assert(tile_get_cell(t2,  9, 10) == OFF);
        assert(tile_get_cell(t2,  9, 11) == OFF);
        assert(tile_get_cell(t2,  9, 12) == OFF);
        assert(tile_get_cell(t2, 10,  9) == OFF);
        assert(tile_get_cell(t2, 10, 10) == ON);
        assert(tile_get_cell(t2, 10, 11) == ON);
        assert(tile_get_cell(t2, 10, 12) == OFF);
        assert(tile_get_cell(t2, 11,  9) == OFF);
        assert(tile_get_cell(t2, 11, 10) == ON);
        assert(tile_get_cell(t2, 11, 11) == ON);
        assert(tile_get_cell(t2, 11, 12) == OFF);
        assert(tile_get_cell(t2, 12,  9) == OFF);
        assert(tile_get_cell(t2, 12, 10) == OFF);
        assert(tile_get_cell(t2, 12, 11) == OFF);
        assert(tile_get_cell(t2, 12, 12) == OFF);

        // and the blinker
        assert(tile_get_cell(t2, 20, 19) == OFF);
        assert(tile_get_cell(t2, 21, 19) == ON);
        assert(tile_get_cell(t2, 22, 19) == OFF);
        assert(tile_get_cell(t2, 20, 20) == OFF);
        assert(tile_get_cell(t2, 21, 20) == ON);
        assert(tile_get_cell(t2, 22, 20) == OFF);
        assert(tile_get_cell(t2, 20, 21) == OFF);
        assert(tile_get_cell(t2, 21, 21) == ON);
        assert(tile_get_cell(t2, 22, 21) == OFF);

        // verify we didn't expand the universe
        assert(t->left == NULL);
        assert(t->right == NULL);
        assert(t->up == NULL);
        assert(t->down == NULL);
}

static void test_universe_evolve_unknown(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;        

        t = universe_find_tile(u, 0, 0, 0, 1);
        // place an unknown block
        
        tile_set_cell(t, 10, 10, UNKNOWN);
        tile_set_cell(t, 10, 11, UNKNOWN);
        tile_set_cell(t, 11, 10, UNKNOWN);
        tile_set_cell(t, 11, 11, UNKNOWN);

        // and a blinker
        tile_set_cell(t, 20, 20, UNKNOWN);
        tile_set_cell(t, 21, 20, UNKNOWN);
        tile_set_cell(t, 22, 20, UNKNOWN);

        // a lone unknown cell
        tile_set_cell(t, 15, 10, UNKNOWN);

        // a pair of unknown cells
        tile_set_cell(t, 20, 10, UNKNOWN);
        tile_set_cell(t, 20, 11, UNKNOWN);

        // a 3x3 block of unknown cells
        tile_set_cell(t, 10, 20, UNKNOWN);
        tile_set_cell(t, 11, 20, UNKNOWN);
        tile_set_cell(t, 12, 20, UNKNOWN);
        tile_set_cell(t, 10, 21, UNKNOWN);
        tile_set_cell(t, 11, 21, UNKNOWN);
        tile_set_cell(t, 12, 21, UNKNOWN);
        tile_set_cell(t, 10, 22, UNKNOWN);
        tile_set_cell(t, 11, 22, UNKNOWN);
        tile_set_cell(t, 12, 22, UNKNOWN);

        universe_evolve_next(u);
        
        t2 = universe_find_tile(u, 1, 0, 0, 0);
        assert(t2);
        assert(t2->prev == t);

        // verify the block
        assert(tile_get_cell(t2,  9,  9) == OFF);
        assert(tile_get_cell(t2,  9, 10) == OFF);
        assert(tile_get_cell(t2,  9, 11) == OFF);
        assert(tile_get_cell(t2,  9, 12) == OFF);
        assert(tile_get_cell(t2, 10,  9) == OFF);
        assert(tile_get_cell(t2, 10, 10) == UNKNOWN);
        assert(tile_get_cell(t2, 10, 11) == UNKNOWN);
        assert(tile_get_cell(t2, 10, 12) == OFF);
        assert(tile_get_cell(t2, 11,  9) == OFF);
        assert(tile_get_cell(t2, 11, 10) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 11) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 12) == OFF);
        assert(tile_get_cell(t2, 12,  9) == OFF);
        assert(tile_get_cell(t2, 12, 10) == OFF);
        assert(tile_get_cell(t2, 12, 11) == OFF);
        assert(tile_get_cell(t2, 12, 12) == OFF);

        // and the blinker
        assert(tile_get_cell(t2, 20, 19) == OFF);
        assert(tile_get_cell(t2, 21, 19) == UNKNOWN);
        assert(tile_get_cell(t2, 22, 19) == OFF);
        assert(tile_get_cell(t2, 20, 20) == OFF);
        assert(tile_get_cell(t2, 21, 20) == UNKNOWN);
        assert(tile_get_cell(t2, 22, 20) == OFF);
        assert(tile_get_cell(t2, 20, 21) == OFF);
        assert(tile_get_cell(t2, 21, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 22, 21) == OFF);

        // the lone cell
        assert(tile_get_cell(t2, 15, 10) == OFF);

        // the pair of cells
        assert(tile_get_cell(t2, 20, 10) == OFF);
        assert(tile_get_cell(t2, 20, 11) == OFF);

        // the 3x3 block of unknown cells
        assert(tile_get_cell(t2, 10, 20) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 20) == UNKNOWN);
        assert(tile_get_cell(t2, 12, 20) == UNKNOWN);
        assert(tile_get_cell(t2, 10, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 12, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 10, 22) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 22) == UNKNOWN);
        assert(tile_get_cell(t2, 12, 22) == UNKNOWN);

        assert(tile_get_cell(t2, 11, 19) == UNKNOWN);
        assert(tile_get_cell(t2,  9, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 13, 21) == UNKNOWN);
        assert(tile_get_cell(t2, 11, 23) == UNKNOWN);

        // verify we didn't expand the universe
        assert(t->left == NULL);
        assert(t->right == NULL);
        assert(t->up == NULL);
        assert(t->down == NULL);        
}

static void test_universe_evolve_expand(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // place a blinker at the very bottom
        tile_set_cell(t, 20, TILE_HEIGHT-1, ON);
        tile_set_cell(t, 21, TILE_HEIGHT-1, ON);
        tile_set_cell(t, 22, TILE_HEIGHT-1, ON);

        universe_evolve_next(u);
        
        t2 = universe_find_tile(u, 1, 0, 0, 0);
        assert(t2);
        assert(t2->prev == t);

        // verify the top 2 cells of the blinker
        assert(tile_get_cell(t2, 21, TILE_HEIGHT-2) == ON);
        assert(tile_get_cell(t2, 21, TILE_HEIGHT-1) == ON);

        // verify that the universe expanded
        tile *t3 = t->down;
        assert(t3);
        tile *t4 = t3->next;
        assert(t4);

        // verify the bottom cell of the blinker
        assert(tile_get_cell(t4, 21, 0) == ON);
}

static void test_universe_r_pentomino(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // A sterner test: 1000 generations of the R-pentomino

        tile_set_cell(t, 20, 20, ON);
        tile_set_cell(t, 20, 21, ON);
        tile_set_cell(t, 21, 21, ON);
        tile_set_cell(t, 22, 21, ON);
        tile_set_cell(t, 21, 22, ON);

        int i;
        for(i=0; i<1000; i++)
                universe_evolve_next(u);

        write_gif(u, 1000, "r-pentomino.gif");
}

static void test_universe_puffer_train(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // An even sterner test: Gosper's puffer train: 
        //
        // .OOO...........OOO
	// O..O..........O..O
	// ...O....OOO......O
	// ...O....O..O.....O
	// ..O....O........O.

        tile_set_cell(t, 10, 11, ON);
        tile_set_cell(t, 10, 12, ON);
        tile_set_cell(t, 10, 13, ON);
        tile_set_cell(t, 10, 25, ON);
        tile_set_cell(t, 10, 26, ON);
        tile_set_cell(t, 10, 27, ON);
        tile_set_cell(t, 11, 10, ON);
        tile_set_cell(t, 11, 13, ON);
        tile_set_cell(t, 11, 24, ON);
        tile_set_cell(t, 11, 27, ON);
        tile_set_cell(t, 12, 13, ON);
        tile_set_cell(t, 12, 18, ON);
        tile_set_cell(t, 12, 19, ON);
        tile_set_cell(t, 12, 20, ON);
        tile_set_cell(t, 12, 27, ON);
        tile_set_cell(t, 13, 13, ON);
        tile_set_cell(t, 13, 18, ON);
        tile_set_cell(t, 13, 21, ON);
        tile_set_cell(t, 13, 27, ON);
        tile_set_cell(t, 14, 12, ON);
        tile_set_cell(t, 14, 17, ON);
        tile_set_cell(t, 14, 26, ON);

        int i;
        for(i=0; i<2500; i++)
                universe_evolve_next(u);

        write_gif(u, 2500, "puffer.gif");
}

static void test_universe_lwss(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // 50,000 generations of a LWSS
        //
        // .OOO
	// O..O
	// ...O
	// ...O
	// ..O.

        tile_set_cell(t, 10, 11, ON);
        tile_set_cell(t, 10, 12, ON);
        tile_set_cell(t, 10, 13, ON);
        tile_set_cell(t, 11, 10, ON);
        tile_set_cell(t, 11, 13, ON);
        tile_set_cell(t, 12, 13, ON);
        tile_set_cell(t, 13, 13, ON);
        tile_set_cell(t, 14, 12, ON);

        int i;
        for(i=0; i<50000; i++)
                universe_evolve_next(u);

        write_gif(u, 50000, "lwss.gif");
}

int main(void) {
        test_universe_basic();
        test_universe_evolve();
        test_universe_evolve_expand();
        test_universe_evolve_unknown();
        test_universe_r_pentomino();
        test_universe_puffer_train();
        test_universe_lwss();
}
