#include "tests_proto.h"

struct { char *nom ; void (*test)() ; } global[] = {
#include "tests_table.h"
} ;
