#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pass.h"
#include "pass1.h"
#include "pass2.h"
#include "ir.h"

extern int yyparse(void);
extern FILE *yyin;

// Print brief CLI usage message to stderr
static void usage(const char* prog) {
  fprintf(stderr, "Usage: %s [-o output.bin] <input.s>\n", prog);
}

// Allocate and return output filename by replacing input's extension with ".bin"
static char* derive_output_name(const char* in) {
  const char* slash = strrchr(in, '/');
  const char* base = slash ? slash + 1 : in;
  size_t n = strlen(base);
  char* out = (char*)malloc(n + 5);
  if (!out) return NULL;

  memcpy(out, base, n + 1);
  char* dot = strrchr(out, '.');
  if (dot && dot != out) *dot = '\0';
  strcat(out, ".bin");
  return out;
}

// Parse args, run pass1, then pass2, handle I/O and errors end to end
int main(int argc, char** argv) {
  const char* in_path = NULL;
  const char* out_path = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 >= argc) {
        usage(argv[0]);
        return 2;
      }
      out_path = argv[++i];
    }
    else if (argv[i][0] == '-') {
      usage(argv[0]);
      return 2;
    }
    else {
      in_path = argv[i];
    }
  }

  if (!in_path) {
    usage(argv[0]);
    return 2;
  }

  FILE* in = fopen(in_path, "r");
  if (!in) {
    fprintf(stderr, "Error: failed to open input '%s': %s\n", in_path, strerror(errno));
    return 1;
  }

  char* auto_out = NULL;
  if (!out_path) {
    auto_out = derive_output_name(in_path);
    if (!auto_out) {
      fprintf(stderr, "Error: failed to allocate output filename\n");
      fclose(in);
      return 1;
    }
    out_path = auto_out;
  }

  pass1_initialize();
  yyin = in;
  int parse_rc = yyparse();
  fclose(in);

  pass1_finalize();
  if (parse_rc != 0) {
    fprintf(stderr, "Error: parse failed (rc=%d)\n", parse_rc);
    free(auto_out);
    return 1;
  }

  FILE* out = fopen(out_path, "wb");
  if (!out) {
    fprintf(stderr, "Error: failed to open output '%s': %s\n", out_path, strerror(errno));
    free(auto_out);
    return 1;
  }

  pass2_initialize();
  pass2_set_output(out);

  bool ok = pass2_run();
  bool closed_ok = (fclose(out) == 0);

  bool final_ok = pass2_finalize();

  if (!closed_ok) {
    fprintf(stderr, "Error: failed to close output '%s': %s\n", out_path, strerror(errno));
    ok = false;
  }

  free(auto_out);
  return (ok && final_ok) ? 0 : 1;
}