#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pass.h"
#include "ir.h"
#include "symtab.h"
#include "pass2.h"

static FILE* outfile = NULL;
static int had_error = 0;