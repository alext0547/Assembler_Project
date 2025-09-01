#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "pass.h"
#include "pass1.h"
#include "pass2.h"
#include "ir.h"
#include "out.h"

extern int yyparse(void);
extern FILE* yyin;
extern int yylex_destroy(void);

// Print brief CLI usage message to stderr
static void usage(const char* prog) {
  fprintf(stderr, "Usage: %s [-o outfile] [-m32|-m64] [-m|-mno-m] [-mc|-mno-c] [-mcompress|-mno-compress] " 
                  "[-felf|-fraw] <input.s>\n   -fraw : raw binary (default, output *.bin)\n   -felf : ELF " 
                  "executable (output *.elf)\n", prog);
}

// Allocate and return output filename by replacing input's extension with ".bin"
static char* derive_output_name(const char* in, out_format_t fmt) {
  const char* slash = strrchr(in, '/');
  #ifdef _WIN32
    const char* bslash = strrchr(in, '\\');
    if (!slash || (bslash && bslash > slash)) slash = bslash;
  #endif
  const char* base = slash ? slash + 1 : in;
  size_t n = strlen(base);
  const char* ext = (fmt == OUT_ELF) ? ".elf" : ".bin";
  char* out = (char*)malloc(n + strlen(ext) + 1);
  if (!out) return NULL;

  memcpy(out, base, n + 1);
  char* dot = strrchr(out, '.');
  if (dot && dot != out) *dot = '\0';
  strcat(out, ext);
  return out;
}

// Parse args, run pass1, then pass2, handle I/O and errors end to end
int main(int argc, char** argv) {
  const char* in_path = NULL;
  const char* out_path = NULL;
  int xlen = 32;
  bool mext = false;
  bool cext = false;
  bool autoC = false;
  static out_format_t parse_out_fmt = OUT_RAW;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      if (++i >= argc) {
        usage(argv[0]);
        return 2;
      }
      out_path = argv[i];
    }
    else if (strcmp(argv[i], "-m64") == 0) {
      xlen = 64;
    }
    else if (strcmp(argv[i], "-m32") == 0) {
      xlen = 32;
    }
    else if (strcmp(argv[i], "-m") == 0) {
      mext = true;
    }
    else if (strcmp(argv[i], "-mno-m") == 0) {
      mext = false;
    }
    else if (strcmp(argv[i], "-mc") == 0) {
      cext = true;
    }
    else if (strcmp(argv[i], "-mno-c") == 0) {
      cext = false;
    }
    else if (strcmp(argv[i], "-mcompress") == 0) {
      autoC = true;
    }
    else if (strcmp(argv[i], "-mno-compress") == 0) {
      autoC = false;
    }
    else if (strcmp(argv[i], "-felf") == 0) {
      parse_out_fmt = OUT_ELF;
    }
    else if (strcmp(argv[i], "-fraw") == 0) {
      parse_out_fmt = OUT_RAW;
    }
    else if (argv[i][0] == '-') {
      usage(argv[0]);
      return 2;
    }
    else if (!in_path) {
      in_path = argv[i];
    }
    else {
      fprintf(stderr, "Error: multiple input files provided ('%s' and '%s')\n", in_path, argv[i]);
      return 2;
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
    auto_out = derive_output_name(in_path, parse_out_fmt);
    if (!auto_out) {
      fprintf(stderr, "Error: failed to allocate output filename\n");
      fclose(in);
      yylex_destroy();
      return 1;
    }
    out_path = auto_out;
  }

  pass_set_arch(xlen, mext);
  pass_set_ext_C(cext);

  if (autoC && !cext) {
    fprintf(stderr, "Warning: -mcompress ignored because C extension is disabled\n");
    autoC = false;
  }
  pass_set_auto_compress(autoC);
  pass1_initialize();
  yyin = in;
  int parse_rc = yyparse();
  fclose(in);
  yylex_destroy();

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

  out_begin(parse_out_fmt, xlen);
  out_set_file(out);

  pass2_initialize();
  pass2_set_output(out);

  bool ok = pass2_run();

  bool final_ok = pass2_finalize();

  uint64_t entry = 0;
  bool ofinish_ok = out_finish(entry);

  bool closed_ok = (fclose(out) == 0);
  if (!closed_ok) {
    fprintf(stderr, "Error: failed to close output '%s': %s\n", out_path, strerror(errno));
    ok = false;
  }

  out_dispose();
  free(auto_out);
  return (ok && final_ok && ofinish_ok && closed_ok) ? 0 : 1;
}