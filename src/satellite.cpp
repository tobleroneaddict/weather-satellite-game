#include "../include/satellite.h"

#define AF1 chip.R1.wr.AF
#define BC1 chip.R1.wr.BC
#define DE1 chip.R1.wr.DE
#define HL1 chip.R1.wr.HL
#define AF2 chip.R2.wr.AF
#define BC2 chip.R2.wr.BC
#define DE2 chip.R2.wr.DE
#define HL2 chip.R2.wr.HL
#define IX chip.R1.wr.IX
#define IY chip.R1.wr.IY
#define SP chip.R1.wr.SP
#define PC chip.PC
#define I chip.I
#define R chip.R
#define IFF1 chip.IFF1
#define IFF2 chip.IFF2
#define IM chip.IM

void SDP::init() {
    Z80RESET(&chip);
    std::cout << "RESET\n";
}