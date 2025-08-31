#include "out.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define EI_NIDENT 16
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define EV_CURRENT 1
#define ET_EXEC 2
#define EM_RISCV 243

#define PT_LOAD 1

#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_STRTAB 3
#define SHT_NOBITS 8

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4

#define EF_RISCV_RVC 0x00000001

static inline uint64_t align_up_u64(uint64_t x, uint64_t a) { return (x + (a-1)) & ~(a-1); }
static inline uint32_t align_up_u32(uint32_t x, uint32_t a) { return (x + (a-1)) & ~(a-1); }

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  uint16_t e_type, e_machine, e_ehsize, e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx;
  uint32_t e_version, e_entry, e_phoff, e_shoff, e_flags;
} elf32_ehdr;

typedef struct {
  uint32_t p_type, p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_flags, p_align;
} elf32_phdr;

typedef struct {
  uint32_t sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize;
} elf32_shdr;

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  uint16_t e_type, e_machine, e_ehsize, e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx;
  uint32_t e_version, e_flags;
  uint64_t e_entry, e_phoff, e_shoff;
} elf64_ehdr;

typedef struct {
  uint32_t p_type, p_flags;
  uint64_t p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_align;
} elf64_phdr;

typedef struct {
  uint32_t sh_name, sh_type, sh_link, sh_info;
  uint64_t sh_flags, sh_addr, sh_offset, sh_size, sh_addralign, sh_entsize;
} elf64_shdr;

typedef struct {
  uint8_t* data;
  size_t len, cap;
} bytevec_t;

typedef struct {
  out_format_t fmt;
  int xlen;
  FILE* fp;
  section_t cur_sect;
  bytevec_t text, data, shstr;
  bool has_rvc;
  uint64_t vaddr_text_base, vaddr_data_base;
} out_ctx_t;

static out_ctx_t G = {0};

// Ensures the byte vector has enough capacity for an additional number of bytes.
static bool bv_reserve(bytevec_t* v, size_t need) {
  if (v->len + need <= v->cap) return true;
  size_t cap2 = v->cap ? v->cap : 256;
  while (cap2 < v->len + need) cap2 *= 2;
  void* p = realloc(v->data, cap2);
  if (!p) return false;
  v->data = (uint8_t*)p;
  v->cap = cap2;
  return true;
}

// Appends a specified number of bytes to the end of the byte vector
static bool bv_append(bytevec_t* v, const void* src, size_t n) {
  if (!bv_reserve(v, n)) return false;
  memcpy(v->data + v->len, src, n);
  v->len += n;
  return true;
}

// Appends a specified number of 0 bytes to the end of the byte vector
static bool bv_zero(bytevec_t* v, size_t n) {
  if (!bv_reserve(v, n)) return false;
  memset(v->data + v->len, 0, n);
  v->len += n;
  return true;
}

// Initializes the global struct G to 0 and fills in base addresses, format, and xlen
void out_begin(out_format_t fmt, int xlen) {
  memset(&G, 0, sizeof(G));
  G.fmt = fmt;
  G.xlen = (xlen == 64) ? 64 : 32;
  G.vaddr_text_base = 0x00100000;
  G.vaddr_data_base = 0x00200000;
}

// Sets the file where the output will be written to f
void out_set_file(FILE* f) { G.fp = f; }

// Sets the current section being processed to sect
void out_begin_section(section_t sect) { G.cur_sect = sect; }

// Sets rvc use to true
void out_mark_rvc(void) { G.has_rvc = true; }

// Writes data to either a raw file or an in memory buffer depending on the output format
static bool sink_write(const void* p, size_t n) {
  if (G.fmt == OUT_RAW) {
    if (!G.fp) return false;
    return fwrite(p, 1, n, G.fp) == n;
  }

  if (G.cur_sect == SEC_TEXT) return bv_append(&G.text, p, n);
  if (G.cur_sect == SEC_DATA) return bv_append(&G.data, p, n);
  return false;
}

// Returns the global text base
uint64_t out_text_base(void) { return G.vaddr_text_base; }

// Writes a specified number of zeroed bytes to an output destination depending on the global state
static bool sink_zeros(size_t n) {
  if (G.fmt == OUT_RAW) {
    if (!G.fp) return false;
    static const uint8_t z[256] = {0};
    while (n) {
      size_t chunk = n > sizeof(z) ? sizeof(z) : n;
      if (fwrite(z, 1, chunk, G.fp) != chunk) return false;
      n -= chunk;
    }
    return true;
  }
  
  if (G.cur_sect == SEC_TEXT) return bv_zero(&G.text, n);
  if (G.cur_sect == SEC_DATA) return bv_zero(&G.data, n);
  return false;
}

// Writes a word into the current output sink in little endian order
bool write_word_le(uint32_t word) { return sink_write(&word, sizeof(word)); }

// Writes a half word into the current output sink in little endian order and marks if the code section has RVC 
// instructions
bool write_half_le(uint16_t half) {
  if (G.fmt == OUT_ELF && G.cur_sect == SEC_TEXT) G.has_rvc = true;
  return sink_write(&half, sizeof(half));
}

// Validates size and writes a value of 8, 16, 32, or 64 bits to the output sink in little endian order.
bool write_value_le(int64_t value, uint32_t size) {
  uint8_t buf[8];
  if (size != 1 && size != 2 && size != 4 && size != 8) return false;
  for (uint32_t i = 0; i < size; i++) buf[i] = (uint8_t)((uint64_t)value >> (8*i));
  return sink_write(buf, size);
}

// Writes a specified number of zero bytes into the current output sink
bool write_zeros(uint32_t n) { return sink_zeros(n); }

// Writes a specified number of bytes to an output file at a given offset
static bool write_at(FILE* f, uint64_t off, const void* p, size_t n) {
  if (!f || (!p && n)) return false;

  #if defined(_WIN32)
    __int64 cur = _ftelli64(f);
    if (cur < 0) return false;
    if (_fseeki64(f, (__int64)off, SEEK_SET) != 0) return false;
  #else
    off_t cur = ftello(f);
    if (cur == (off_t)-1) return false;
    if (fseeko(f, (off_t)off, SEEK_SET) != 0) return false;
  #endif

  const uint8_t* buf = (const uint8_t*)p;
  size_t left = n;
  while (left) {
    size_t wrote = fwrite(buf, 1, left, f);
    if (wrote == 0) {
      #if defined(_WIN32)
        (void)_fseeki64(f, cur, SEEK_SET);
      #else
        (void)fseeko(f, cur, SEEK_SET);
      #endif
        return false;
    }
    buf += wrote;
    left -= wrote;
  }

  #if defined(_WIN32)
    if (_fseeki64(f, cur, SEEK_SET) != 0) return false;
  #else
    if (fseeko(f, cur, SEEK_SET) != 0) return false;
  #endif
    return true;
}

// Completes the output file, generating either an ELF binary with all headers or a raw binary, based on the 
// specified format
bool out_finish(uint64_t entry_addr) {
  if (G.fmt == OUT_RAW) {
    return (G.fp != NULL);
  }

  const bool is64 = (G.xlen == 64);
  const uint32_t text_align = 0x1000;
  const uint32_t data_align = 0x1000;
  const uint32_t sh_align = is64 ? 8 : 4;
  const uint64_t text_size = G.text.len;
  const uint64_t data_size = G.data.len;

  if (!G.fp) return false;

  const uint64_t ehdr_sz = is64 ? sizeof(elf64_ehdr) : sizeof(elf32_ehdr);
  const uint64_t phdr_sz = is64 ? sizeof(elf64_phdr) : sizeof(elf32_phdr);
  const uint64_t shdr_sz = is64 ? sizeof(elf64_shdr) : sizeof(elf32_shdr);

  uint16_t phnum = 0;
  if (text_size) phnum++;
  if (data_size) phnum++;

  uint16_t shnum = (uint16_t)(2 + (text_size ? 1 : 0) + (data_size ? 1 : 0));

  uint64_t off = ehdr_sz + phdr_sz * phnum;
  uint64_t text_off = 0, data_off = 0, shstr_off = 0, shoff = 0;

  if (text_size) {
    off = is64 ? align_up_u64(off, text_align) : align_up_u32((uint32_t)off, text_align);
    text_off = off;
    off += text_size;
  }
  if (data_size) {
    off = is64 ? align_up_u64(off, data_align) : align_up_u32((uint32_t)off, data_align);
    data_off = off;
    off += data_size;
  }

  bytevec_t shstr = (bytevec_t){0};
  (void)bv_append(&shstr, "\0", 1);
  uint32_t nm_text = 0, nm_data = 0, nm_shstr = 0;
  if (text_size) {
    nm_text = (uint32_t)shstr.len;
    (void)bv_append(&shstr, ".text\0", 6);
  }
  if (data_size) {
    nm_data = (uint32_t)shstr.len;
    (void)bv_append(&shstr, ".data\0", 6);
  }
  nm_shstr = (uint32_t)shstr.len;
  (void)bv_append(&shstr, ".shstrtab\0", 10);

  shstr_off = off;
  off += shstr.len;

  off = is64 ? align_up_u64(off, sh_align) : align_up_u32((uint32_t)off, sh_align);
  shoff = off;
  off += shdr_sz * shnum;

  if (text_size && !write_at(G.fp, text_off, G.text.data, G.text.len)) {
    free(shstr.data);
    return false;
  }
  if (data_size && !write_at(G.fp, data_off, G.data.data, G.data.len)) {
    free(shstr.data);
    return false;
  }
  if (!write_at(G.fp, shstr_off, shstr.data, shstr.len)) {
    free(shstr.data);
    return false;
  }

  if (phnum > 0) {
    if (is64) {
      elf64_phdr ph[2];
      memset(ph, 0, sizeof(ph));
      uint16_t idx = 0;
      if (text_size) {
        ph[idx].p_type = PT_LOAD;
        ph[idx].p_flags = PF_R | PF_X;
        ph[idx].p_offset = text_off;
        ph[idx].p_vaddr = G.vaddr_text_base;
        ph[idx].p_paddr = G.vaddr_text_base;
        ph[idx].p_filesz = text_size;
        ph[idx].p_memsz = text_size;
        ph[idx].p_align = text_align;
        idx++;
      }
      if (data_size) {
        ph[idx].p_type = PT_LOAD;
        ph[idx].p_flags = PF_R | PF_W;
        ph[idx].p_offset = data_off;
        ph[idx].p_vaddr = G.vaddr_data_base;
        ph[idx].p_paddr = G.vaddr_data_base;
        ph[idx].p_filesz = data_size;
        ph[idx].p_memsz = data_size;
        ph[idx].p_align = data_align;
        idx++;
      }
      if (!write_at(G.fp, ehdr_sz, ph, phnum * sizeof(elf64_phdr))) {
        free(shstr.data);
        return false;
      }
    }
    else {
      elf32_phdr ph[2];
      memset(ph, 0, sizeof(ph));
      uint16_t idx = 0;
      if (text_size) {
        ph[idx].p_type = PT_LOAD;
        ph[idx].p_flags = PF_R | PF_X;
        ph[idx].p_offset = (uint32_t)text_off;
        ph[idx].p_vaddr = (uint32_t)G.vaddr_text_base;
        ph[idx].p_paddr = (uint32_t)G.vaddr_text_base;
        ph[idx].p_filesz = (uint32_t)text_size;
        ph[idx].p_memsz = (uint32_t)text_size;
        ph[idx].p_align = text_align;
        idx++;
      }
      if (data_size) {
        ph[idx].p_type = PT_LOAD;
        ph[idx].p_flags = PF_R | PF_W;
        ph[idx].p_offset = (uint32_t)data_off;
        ph[idx].p_vaddr = (uint32_t)G.vaddr_data_base;
        ph[idx].p_paddr = (uint32_t)G.vaddr_data_base;
        ph[idx].p_filesz = (uint32_t)data_size;
        ph[idx].p_memsz = (uint32_t)data_size;
        ph[idx].p_align = data_align;
        idx++;
      }
      if (!write_at(G.fp, ehdr_sz, ph, phnum * sizeof(elf32_phdr))) {
        free(shstr.data);
        return false;
      }
    }
  }

  if (is64) {
    elf64_shdr sh;
    memset(&sh, 0, sizeof(sh));

    if (!write_at(G.fp, shoff + 0 * shdr_sz, &sh, sizeof(sh))) {
      free(shstr.data);
      return false;
    }
    uint16_t si = 1;

    if (text_size) {
      memset(&sh, 0, sizeof(sh));
      sh.sh_name = nm_text;
      sh.sh_type = SHT_PROGBITS;
      sh.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
      sh.sh_addr = G.vaddr_text_base;
      sh.sh_offset = text_off;
      sh.sh_size = text_size;
      sh.sh_addralign = 2;
      if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
        free(shstr.data);
        return false;
      }
      si++;
    }
    if (data_size) {
      memset(&sh, 0, sizeof(sh));
      sh.sh_name = nm_data;
      sh.sh_type = SHT_PROGBITS;
      sh.sh_flags = SHF_ALLOC | SHF_WRITE;
      sh.sh_addr = G.vaddr_data_base;
      sh.sh_offset = data_off;
      sh.sh_size = data_size;
      sh.sh_addralign = 4;
      if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
        free(shstr.data);
        return false;
      }
      si++;
    }
    
    memset(&sh, 0, sizeof(sh));
    sh.sh_name = nm_shstr;
    sh.sh_type = SHT_STRTAB;
    sh.sh_offset = shstr_off;
    sh.sh_size = shstr.len;
    sh.sh_addralign = 1;
    if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
      free(shstr.data);
      return false;
    }
  }
  else {
    elf32_shdr sh;
    memset(&sh, 0, sizeof(sh));
    if (!write_at(G.fp, shoff + 0 * shdr_sz, &sh, sizeof(sh))) {
      free(shstr.data);
      return false;
    }
    uint16_t si = 1;

    if (text_size) {
      memset(&sh, 0, sizeof(sh));
      sh.sh_name = nm_text;
      sh.sh_type = SHT_PROGBITS;
      sh.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
      sh.sh_addr = (uint32_t)G.vaddr_text_base;
      sh.sh_offset = (uint32_t)text_off;
      sh.sh_size = (uint32_t)text_size;
      sh.sh_addralign = 2;
      if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
        free(shstr.data);
        return false;
      }
      si++;
    }
    if (data_size) {
      memset(&sh, 0, sizeof(sh));
      sh.sh_name = nm_data;
      sh.sh_type = SHT_PROGBITS;
      sh.sh_flags = SHF_ALLOC | SHF_WRITE;
      sh.sh_addr = (uint32_t)G.vaddr_data_base;
      sh.sh_offset = (uint32_t)data_off;
      sh.sh_size = (uint32_t)data_size;
      sh.sh_addralign = 4;
      if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
        free(shstr.data);
        return false;
      }
      si++;
    }
    memset(&sh, 0, sizeof(sh));
    sh.sh_name = nm_shstr;
    sh.sh_type = SHT_STRTAB;
    sh.sh_offset = (uint32_t)shstr_off;
    sh.sh_size = (uint32_t)shstr.len;
    sh.sh_addralign = 1;
    if (!write_at(G.fp, shoff + si * shdr_sz, &sh, sizeof(sh))) {
      free(shstr.data);
      return false;
    }
  }

  const uint32_t flags = (G.has_rvc ? EF_RISCV_RVC : 0);
  const uint64_t default_entry = text_size ? G.vaddr_text_base : G.vaddr_data_base;

  if (is64) {
    elf64_ehdr eh;
    memset(&eh, 0, sizeof(eh));
    eh.e_ident[0] = ELFMAG0;
    eh.e_ident[1] = ELFMAG1;
    eh.e_ident[2] = ELFMAG2;
    eh.e_ident[3] = ELFMAG3;
    eh.e_ident[4] = ELFCLASS64;
    eh.e_ident[5] = ELFDATA2LSB;
    eh.e_ident[6] = EV_CURRENT;

    eh.e_type = ET_EXEC;
    eh.e_machine = EM_RISCV;
    eh.e_version = EV_CURRENT;
    eh.e_entry = entry_addr ? entry_addr : default_entry;
    eh.e_phoff = (phnum ? ehdr_sz : 0);
    eh.e_shoff = shoff;
    eh.e_flags = flags;
    eh.e_ehsize = sizeof(eh);
    eh.e_phentsize = sizeof(elf64_phdr);
    eh.e_phnum = phnum;
    eh.e_shentsize = sizeof(elf64_shdr);
    eh.e_shnum = shnum;
    eh.e_shstrndx = shnum - 1;

    if (!write_at(G.fp, 0, &eh, sizeof(eh))) {
      free(shstr.data);
      return false;
    }
  }
  else {
    elf32_ehdr eh;
    memset(&eh, 0, sizeof(eh));
    eh.e_ident[0] = ELFMAG0;
    eh.e_ident[1] = ELFMAG1;
    eh.e_ident[2] = ELFMAG2;
    eh.e_ident[3] = ELFMAG3;
    eh.e_ident[4] = ELFCLASS32;
    eh.e_ident[5] = ELFDATA2LSB;
    eh.e_ident[6] = EV_CURRENT;

    eh.e_type = ET_EXEC;
    eh.e_machine = EM_RISCV;
    eh.e_version = EV_CURRENT;
    eh.e_entry = (uint32_t)(entry_addr ? entry_addr : default_entry);
    eh.e_phoff = (uint32_t)(phnum ? ehdr_sz : 0);
    eh.e_shoff = (uint32_t)shoff;
    eh.e_flags = flags;
    eh.e_ehsize = sizeof(eh);
    eh.e_phentsize = sizeof(elf32_phdr);
    eh.e_phnum = phnum;
    eh.e_shentsize = sizeof(elf32_shdr);
    eh.e_shnum = shnum;
    eh.e_shstrndx = shnum - 1;

    if (!write_at(G.fp, 0, &eh, sizeof(eh))) {
      free(shstr.data);
      return false;
    }
  }

  free(shstr.data);
  return true;
}