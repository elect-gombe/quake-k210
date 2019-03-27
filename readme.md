# Quake for k210
Dualshock2 is now supported! I've tested E1 at shareware mode.
The framerate is very stable(25~70fps), and it definitely playable.

Note that controls and brightness can change at options menu.
## Default key configulation
### buttons
- Start ... Enter
- Select ... Esc
- directional button ... Arrow
Other buttons are as it's name. (L1, L2, L3, R1, ..., Circle, ...)

### Analog sticks
This functions is fixed by code.
- Left stick ... Move
- Right stick ... Camera

## build
- Stack memory is expanded, 
- compile error is supressed by compile flags
visit `additionalparts/readme.md` to build this program.

## install
1. copy id1/pak?.pak to your SD. You can DL the package file in the official Quake shareware, or use full package file(s).
2. connect your dualshock2 to k210.
See `source/k210.c`.
```c
    fpioa_set_function(18, FUNC_GPIOHS0 + DS2_CS);   //Dualshock2 ss
    fpioa_set_function(19, FUNC_GPIOHS0 + DS2_CLK); //clk
    fpioa_set_function(21, FUNC_GPIOHS0 + DS2_MOSI); //mosi--DO/CMD
    fpioa_set_function(20, FUNC_GPIOHS0 + DS2_MISO); //miso--DI/DAT
```
3. Set each of jump, Atack and switch weapon function to the button(s).

## implementation note
- AI memory is used in general purpose.

## todo
- [X] fix stack setup. use `_sp0`, `_sp1`, etc. => done
- [X] key input, use game controller. => done
- [ ] Sound controller
- [ ] save (file write functions)
- [ ] key asign (plz set up your configulation at option menu.)
- [ ] Adjust stick sensitiveness

## Credits
### original crtQuake (3DS port)
Felipe Izzo (MasterFeizz) - Main developer
Rinnegatamante - Sound enhancements
## Thanks to
Id Software - Source code of Quake
