#include <stdlib.h>
#include <assert.h>
#include "universe.h"
#include "readwrite.h"

int main(int argc, char *argv[]) {

        universe *u = read_text(argv[1]);

        find_still_life(u);

        return 0;
}

