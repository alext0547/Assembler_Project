# RISC-V Assembler (ELF/RAW)

A small, educational RISC‑V assembler that parses a subset of **RV32I/RV64I** with optional **M** (mul/div) and **C** (compressed) extensions. It emits either:

- a raw flat binary (`.bin`, default), or
- a RISC‑V **ELF EXEC** (`.elf`) with two `PT_LOAD` segments (`.text`, `.data`).

Built with a Flex+Bison frontend, two backend passes, and a tiny ELF writer.

---

## Features

- RV32I / RV64I instruction encoding
- Optional **M** (`-m`) and **C** (`-mc`) extensions
- Optional **auto‑compression** pass (`-mcompress`, **requires `-mc`**)
- Data directives: `.byte .half .word .dword .ascii .asciiz .space .align`
- Sections: `.text`, `.data`
- Branch/jump & PC‑relative fixups (HI20/LO12, branch offsets, JAL targets)

---

## Output formats

- **RAW** (default): flat stream of the current section.
- **ELF EXEC**:
  - `.text` (RX) @ `0x0010_0000`, `.data` (RW) @ `0x0020_0000`
  - Sets `EF_RISCV_RVC` if compressed code is present
  - Entry point: `_start` if defined, else `.text` base
  - Section table: `.text`, `.data`, `.shstrtab` (no symbols/relocs)

---

## Quick start

### Prerequisites

- `gcc` (or `clang`), `make`
- `flex`, `bison`
- `readelf`/`objdump` (binutils or LLVM tools)
- Optional: `valgrind`

### Build

```bash
make
```

Produces the executable: `./assembler`.

### Usage

```text
assembler [-o output.bin] [-m32|-m64] [-m|-mno-m] [-mc|-mno-c] \
          [-mcompress|-mno-compress] [-felf|-fraw] <input.s>

  -fraw          Raw binary (default; output *.bin)
  -felf          ELF executable (output *.elf)
  -m32|-m64      Select RV32 or RV64 codegen (default RV32)
  -m|-mno-m      Enable/disable M extension
  -mc|-mno-c     Enable/disable C extension (compressed)
  -mcompress     Try to auto-compress eligible instructions (requires -mc)
  -o <path>      Output file path; if omitted, name is derived from input
```

> **Note**
> The first non‑flag argument is treated as the input file. If you want to set the output name, use `-o`. Avoid passing an extra filename positionally — it will be treated as a 2nd input and rejected.

### Examples

### Tests

Run all 15 tests:

```bash
make test
make test-valgrind                          # or: VALGRIND=1 make test
```

### RAW binary (default)

```bash
./assembler Assembler/testing/test1.s        # emits test1.bin
xxd test1.bin | head
```

---

## Project layout

```Assembler/
  include/
    pass.h pass1.h pass2.h ir.h out.h ...
  src/
    main.c          # CLI, setup, passes, I/O
    pass.c          # global pass configuration (arch, ext, options)
    pass1.c         # parse + IR build
    pass2.c         # layout, fixups, encoding, emission
    ir.c            # IR helpers
    symtab.c        # symbol table
    c_rules.c       # compressed instruction selection
    out.c           # sinks + ELF writer
    assembler.l     # Flex lexer
    assembler.y     # Bison grammar
  testing/
    test1.s ...
Makefile
```

---

## ELF output details

### Virtual addresses

- `.text` base: `0x0010_0000`
- `.data` base: `0x0020_0000`

### Program headers

- `PT_LOAD` for `.text` (`R|X`, aligned `0x1000`)
- `PT_LOAD` for `.data` (`R|W`, aligned `0x1000`)

### Sections

- `.text`, `.data`, `.shstrtab` only (no symbol/reloc tables)

### Entry

- `_start` symbol if present; otherwise `.text` base

> This produces a final executable, not a relocatable `.o`. Linking against libraries is out of scope.

---

## Testing & tooling

### ELF sanity checks

```bash
readelf -hSWl out.elf
riscv64-linux-gnu-objdump -d out.elf | less
```

### Memory checks (Valgrind)

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  ./assembler -m64 -mc -mcompress -felf Assembler/testing/test13.s
```

Expected: `All heap blocks were freed — no leaks are possible.`

### Optional: sanitizers

```bash
make clean
make CFLAGS='-std=c11 -O2 -g -Wall -Wextra \
             -fsanitize=address,undefined -fno-omit-frame-pointer'
ASAN_OPTIONS=detect_leaks=1 \
  ./assembler -m64 -felf Assembler/testing/test1.s
```

---

## How it works

1. **Front‑end (Flex/Bison)**: tokenizes and parses source into an **IR**.
2. **Pass 1**: builds the symbol table, computes section layout, collects fixups.
3. **Pass 2**: resolves labels/relocations (branch offsets, J/JAL targets, AUIPC/LUI+ADDI pairs), applies optional auto‑compression, and emits bytes via the output sink.
4. **Output**:
   - **RAW**: write bytes exactly as produced per section.
   - **ELF**: buffer `.text`/`.data`, then build headers & sections and write a valid **ELF EXEC** (sets `EF_RISCV_RVC` when appropriate).

---

## Limitations

- Emits **EXEC** ELF only (no relocatable `.o`, no dynamic linking)
- No symbol table / relocation sections in output
- Fixed VA bases for `.text`/`.data`
- Little‑endian only
- Limited directive set (see Features)
- Not a drop‑in replacement for `riscv64-unknown-elf-as`

---

## Tested on

- Linux x86_64 (GCC/Clang), WSL Ubuntu
- Flex ≥ 2.6, Bison ≥ 3.8
- binutils (readelf/objdump), Valgrind ≥ 3.25

> Other UNIX‑like environments should work with equivalent toolchains.

---

## Makefile targets

- `all` (default): build `assembler`
- `run`: runs `./assembler $(RUN_ARGS)`
- `clean`: remove objects, generated parser/lexer sources, and binary

Example:

```bash
make run RUN_ARGS='-m64 -m -mc -mcompress -felf -o test13.elf Assembler/testing/test13.s'
```

---

## Contributing

Contributions are welcome! Open issues or submit pull requests.

---

## License

MIT — see [`LICENSE`](LICENSE).
