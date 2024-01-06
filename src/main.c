#include "sage.h"
#include "checker.h"

int main(int argc, char** argv){
    (void)argc;
    (void)argv;
    Sage* s = Sage_create("BlockFun", 800, 600);
    Checker_create(s, 0);

    Sage_run(s);
    Sage_destroy(s);
    return 0;
}
