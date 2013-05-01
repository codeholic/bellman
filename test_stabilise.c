#include <stdlib.h>
#include <assert.h>
#include "universe.h"
#include "readwrite.h"

static void test_stabilise_simple(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2, *t3;

        t = universe_find_tile(u, 0, 0, 0, 1);
        universe_evolve_next(u);
        t2 = universe_find_tile(u, 1, 0, 0, 1);
        universe_evolve_next(u);
        t3 = universe_find_tile(u, 2, 0, 0, 1);

        // place a block
        tile_set_cell(t, 4, 4, ON);
        tile_set_cell(t, 4, 5, ON);
        tile_set_cell(t, 5, 4, ON);
        tile_set_cell(t, 5, 5, UNKNOWN_STABLE);

        // place a boat
        tile_set_cell(t, 10, 10, ON);
        tile_set_cell(t, 10, 11, ON);
        tile_set_cell(t, 11, 10, ON);
        tile_set_cell(t, 11, 12, ON);
        tile_set_cell(t, 12, 11, ON);
        tile_set_cell(t, 11, 11, UNKNOWN_STABLE);

        // a 3x3 block of unknown cells
        tile_set_cell(t, 10, 20, UNKNOWN_STABLE);
        tile_set_cell(t, 11, 20, UNKNOWN_STABLE);
        tile_set_cell(t, 12, 20, UNKNOWN_STABLE);
        tile_set_cell(t, 10, 21, UNKNOWN_STABLE);
        tile_set_cell(t, 11, 21, UNKNOWN_STABLE);
        tile_set_cell(t, 12, 21, UNKNOWN_STABLE);
        tile_set_cell(t, 10, 22, UNKNOWN_STABLE);
        tile_set_cell(t, 11, 22, UNKNOWN_STABLE);
        tile_set_cell(t, 12, 22, UNKNOWN_STABLE);

        // a maybe-eater
        //
        // ??
        // ? ?
        //   ?
        //   ??
        tile_set_cell(t, 15, 5, UNKNOWN_STABLE);
        tile_set_cell(t, 16, 5, UNKNOWN_STABLE);
        tile_set_cell(t, 15, 6, UNKNOWN_STABLE);
        tile_set_cell(t, 17, 6, UNKNOWN_STABLE);
        tile_set_cell(t, 17, 7, UNKNOWN_STABLE);
        tile_set_cell(t, 17, 8, UNKNOWN_STABLE);
        tile_set_cell(t, 18, 8, UNKNOWN_STABLE);

        int rv = tile_stabilise_3state(t, t2);
        assert(rv == ACTIVE);

        rv = tile_stabilise_3state(t2, t3);
        assert(rv == 0);

        assert(tile_get_cell(t2, 5, 5) == ON);
        assert(tile_get_cell(t2, 11, 11) == OFF);

        // the 3x3 block of unknown cells
        assert(tile_get_cell(t2, 10, 20) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 11, 20) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 12, 20) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 10, 21) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 11, 21) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 12, 21) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 10, 22) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 11, 22) == UNKNOWN_STABLE);
        assert(tile_get_cell(t2, 12, 22) == UNKNOWN_STABLE);

}

static void test_stabilise_abort(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2, *t3;

        t = universe_find_tile(u, 0, 0, 0, 1);
        universe_evolve_next(u);
        t2 = universe_find_tile(u, 1, 0, 0, 1);

        // place a block
        tile_set_cell(t, 4, 4, ON);
        tile_set_cell(t, 4, 5, ON);
        tile_set_cell(t, 5, 4, ON);
        tile_set_cell(t, 5, 5, OFF);

        int rv = tile_stabilise_3state(t, t->next);
        assert(rv & ABORT);
}

static void test_stabilise_abort2(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2, *t3;

        t = universe_find_tile(u, 0, 0, 0, 1);
        universe_evolve_next(u);
        t2 = universe_find_tile(u, 1, 0, 0, 1);

        // line of 2 cells
        tile_set_cell(t, 4, 4, ON);
        tile_set_cell(t, 4, 5, ON);

        int rv = tile_stabilise_3state(t, t->next);

        assert(rv & ABORT);
}

static void test_find_still(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2, *t3;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // place a block
        tile_set_cell(t, 4, 4, ON);
        tile_set_cell(t, 4, 5, ON);
        tile_set_cell(t, 5, 4, ON);
        tile_set_cell(t, 5, 5, UNKNOWN_STABLE);

        universe *u2 = find_still_life(u);
        assert(u2);

        t = universe_find_tile(u2, 0, 0, 0, 0);

        assert(tile_get_cell(t, 4, 4) == ON);
        assert(tile_get_cell(t, 4, 5) == ON);
        assert(tile_get_cell(t, 5, 4) == ON);
        assert(tile_get_cell(t, 5, 5) == ON);

}

static void test_find_still_2(void) {
        universe *u = universe_new(OFF);
        tile *t, *t2, *t3;

        t = universe_find_tile(u, 0, 0, 0, 1);

        // place a block
        tile_set_cell(t, 4, 4, UNKNOWN_STABLE);
        tile_set_cell(t, 4, 5, UNKNOWN_STABLE);
        tile_set_cell(t, 5, 4, UNKNOWN_STABLE);
        tile_set_cell(t, 5, 5, UNKNOWN_STABLE);

        universe *u2 = find_still_life(u);
        assert(u2);

        t = universe_find_tile(u2, 0, 0, 0, 0);

        assert(tile_get_cell(t, 4, 4) == OFF);
        assert(tile_get_cell(t, 4, 5) == OFF);
        assert(tile_get_cell(t, 5, 4) == OFF);
        assert(tile_get_cell(t, 5, 5) == OFF);

}

static void test_find_still_minimal_1(void) {
        universe *u;
        tile *t, *t2, *t3;

        u = read_text("test-cases/still-minimal-1.lif");

        universe *u2 = find_still_life(u);
        assert(u2);

        t = universe_find_tile(u2, 0, 0, 0, 0);
        assert(tile_get_cell(t, 7, 4) == ON);
        assert(tile_get_cell(t, 7, 5) == ON);
        assert(tile_get_cell(t, 8, 5) == ON);
}

int main(void) {
        test_stabilise_simple();
        test_stabilise_abort();
        test_stabilise_abort2();
        test_find_still();
        test_find_still_2();
        test_find_still_minimal_1();
}
