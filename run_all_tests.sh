#!/usr/bin/env bash
# Run all tests under Assembler/testing and verify outputs.
# Per-test flags:
#   (a) first "# FLAGS: ..." in the .s file, or
#   (b) Assembler/testing/flags.lst line: "testname: <flags>"
# Default flags if none provided: -m32 -felf

set -Eeuo pipefail

TEST_DIR=${1:-Assembler/testing}
OUT_DIR=${OUT_DIR:-out}
ASM=${ASM:-./assembler}
DEFAULT_FLAGS=${DEFAULT_FLAGS:-"-m32 -felf"}
MANIFEST="$TEST_DIR/flags.lst"

# --- Valgrind (opt-in) ---
# Enable with: VALGRIND=1 ./run_all_tests.sh   or  VALGRIND=1 make test
VALGRIND=${VALGRIND:-0}
# Fail the test if Valgrind reports errors or leaks:
VG_FLAGS=${VG_FLAGS:-"--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=99 --errors-for-leak-kinds=definite,indirect,possible"}

mkdir -p "$OUT_DIR"
pass=0; fail=0

red()   { printf "\033[31m%s\033[0m\n" "$*"; }
green() { printf "\033[32m%s\033[0m\n" "$*"; }
yellow(){ printf "\033[33m%s\033[0m\n" "$*"; }

has_errors_in_log() { local log="$1"
  grep -Eiq '(^|[^A-Za-z])error:|segmentation fault' "$log"
}

# Extract hex address that follows "PROGBITS" in `readelf -SW` line.
addr_after_progbits() {
  awk 'match($0,/PROGBITS[[:space:]]+([0-9A-Fa-f]+)/,m){print m[1]; exit}'
}

# Run the assembler (optionally under Valgrind)
run_assembler() {
  local t="$1" flags="$2" out="$3" log="$4" base="$5"
  if [[ "$VALGRIND" == "1" ]]; then
    local vglog="$OUT_DIR/$base.valgrind.log"
    # Valgrind writes its own log so assembler output stays in $log
    valgrind $VG_FLAGS --log-file="$vglog" \
      $ASM $flags -o "$out" "$t" >"$log" 2>&1
    return $?
  else
    $ASM $flags -o "$out" "$t" >"$log" 2>&1
    return $?
  fi
}

valgrind_failed() {
  # This is a safety net if VG_FLAGS didn't convert leaks to errors.
  # Treat non-zero ERROR SUMMARY or any "definitely lost" > 0 as a fail.
  local vglog="$1"
  [[ -f "$vglog" ]] || return 1
  grep -Eq "ERROR SUMMARY: [1-9]" "$vglog" && return 0
  grep -Eq "definitely lost:\s*[1-9]" "$vglog" && return 0
  return 1
}

verify_elf() {
  local elf="$1"
  local flags="$2"
  local base
  base="$(basename -- "$elf")"
  local hdr="$OUT_DIR/$base.readelf.h.txt"
  local sec="$OUT_DIR/$base.readelf.S.txt"
  local ph="$OUT_DIR/$base.readelf.l.txt"

  readelf -h  "$elf" >"$hdr"
  readelf -SW "$elf" >"$sec"
  readelf -l  "$elf" >"$ph"

  # Machine + Class
  grep -q "Machine:[[:space:]]*RISC-V" "$hdr" || { red "  FAIL: not RISC-V"; return 1; }
  if [[ "$flags" =~ -m64 ]]; then
    grep -q "Class:[[:space:]]*ELF64" "$hdr" || { red "  FAIL: expected ELF64"; return 1; }
  else
    grep -q "Class:[[:space:]]*ELF32" "$hdr" || { red "  FAIL: expected ELF32"; return 1; }
  fi

  # .text address (required)
  local text_addr
  text_addr=$(grep -E '\[[[:space:]]*[0-9]+\][[:space:]]+\.text[[:space:]]+PROGBITS' "$sec" | addr_after_progbits || true)
  [[ -n "${text_addr:-}" ]] || { red "  FAIL: missing .text"; return 1; }
  [[ "$text_addr" =~ ^0*00100000$ ]] || { red "  FAIL: .text base not 0x00100000 (got $text_addr)"; return 1; }

  # .data address (optional: only check if present)
  local data_addr
  if grep -qE '\[[[:space:]]*[0-9]+\][[:space:]]+\.data[[:space:]]+PROGBITS' "$sec"; then
    data_addr=$(grep -E '\[[[:space:]]*[0-9]+\][[:space:]]+\.data[[:space:]]+PROGBITS' "$sec" | addr_after_progbits || true)
    [[ -n "${data_addr:-}" ]] || { red "  FAIL: .data present but address parse failed"; return 1; }
    [[ "$data_addr" =~ ^0*00200000$ ]] || { red "  FAIL: .data base not 0x00200000 (got $data_addr)"; return 1; }
  fi

  # Program headers: accept 1+ LOAD (text-only builds may have 1)
  local load_count
  load_count=$(grep -cE '^[[:space:]]*LOAD[[:space:]]' "$ph" || true)
  (( load_count >= 1 )) || { red "  FAIL: expected >=1 PT_LOAD segments"; return 1; }

  # If compressed requested, warn if ELF flags lack RVC
  if [[ "$flags" =~ -mc ]] || [[ "$flags" =~ -mcompress ]]; then
    grep -Eiq "Flags:.*RVC" "$hdr" || yellow "  WARN: -mc requested but ELF flags lack RVC"
  fi

  [[ -s "$elf" ]] || { red "  FAIL: empty ELF"; return 1; }
  return 0
}

for t in "$TEST_DIR"/test*.s; do
  [[ -f "$t" ]] || continue
  base=$(basename "$t" .s)
  flags="$DEFAULT_FLAGS"

  if grep -m1 -E '^# *FLAGS:' "$t" >/dev/null 2>&1; then
  # Use [[:space:]] (POSIX) and strip any trailing CR in case of CRLF
    flags=$(grep -m1 -E '^# *FLAGS:' "$t" \
            | sed -E 's/^# *FLAGS:[[:space:]]*//' \
            | tr -d '\r')
  elif [[ -f "$MANIFEST" ]] && grep -E "^$base[[:space:]]*:" "$MANIFEST" >/dev/null; then
    flags=$(grep -E "^$base[[:space:]]*:" "$MANIFEST" \
            | sed -E "s/^$base[[:space:]]*:[[:space:]]*//" \
            | tr -d '\r')
  fi


  ext=bin; [[ "$flags" =~ -felf ]] && ext=elf
  out="$OUT_DIR/$base.$ext"
  log="$OUT_DIR/$base.log"

  echo "==> $base.s"
  echo "    FLAGS: $flags"

  set +e
  run_assembler "$t" "$flags" "$out" "$log" "$base"
  rc=$?
  set -e

  if [[ $rc -ne 0 || $(has_errors_in_log "$log"; echo $?) -eq 0 ]]; then
    red "  FAIL: assemble $base (rc=$rc). See $log"
    # show a quick hint
    tail -n 20 "$log" | sed 's/^/    | /'
    # If Valgrind was used, also show its log tail to point at the root cause
    if [[ "$VALGRIND" == "1" && $rc -ne 0 ]]; then
      vglog="$OUT_DIR/$base.valgrind.log"
      if [[ -f "$vglog" ]]; then
        echo "    | ---- Valgrind last 30 lines ----"
        tail -n 30 "$vglog" | sed 's/^/    | /'
      fi
    fi
    ((fail++)) || true
    continue
  fi

  if [[ "$ext" = "elf" ]]; then
    if ! verify_elf "$out" "$flags"; then
      red "  FAIL: verify $base"
      ((fail++)) || true
      continue
    fi
  else
    [[ -s "$out" ]] || { red "  FAIL: empty raw binary"; ((fail++)) || true; continue; }
  fi

    # If assembler succeeded and we ran under Valgrind, fail on reported issues.
  if [[ "$VALGRIND" == "1" ]]; then
    vglog="$OUT_DIR/$base.valgrind.log"
    if valgrind_failed "$vglog"; then
      red "  FAIL: valgrind reported issues (see $vglog)"
      ((fail++)) || true
      continue
    fi
  fi

  green "  PASS: $base"
  ((pass++)) || true
done

echo
echo "Summary: PASS=$pass FAIL=$fail"
exit $(( fail > 0 ))
