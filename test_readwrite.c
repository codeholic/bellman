#include <stdlib.h>
#include <assert.h>
#include "universe.h"
#include "readwrite.h"

static void test_read_cordership(void) {
        universe *u;

        u = read_text("test-cases/3enginecordership_105.lif");

        write_gif(u, u->n_gens - 1, "cs.gif");

        universe_free(u);
}

int main(void) {
        test_read_cordership();
}
