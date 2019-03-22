### stack memory 
This program needs lot of stack memory(need 512KB, but 32KB in original SDK configulation).
Use these files to expand stack memory.

crt.S ... new startup routine, override `lib/bsp/crt.S`
kendryte.ld ... new linker script, override `lds/kendryte.ld`

### compile flags
This source code is too old, so use new `compile-flags.cmake` in the `cmake` directory.
