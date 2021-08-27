#include <mod/amlmod.h>

MYMOD(net.rusjj.gtasa.vertexweight, GTA:SA Vertex Weight Fix, 1.0, RusJJ (original by iFarbod))
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGTASA = 0;

extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");

    aml->Unprot(pGTASA + 0x1C8064, sizeof(char));
    *(char*)(pGTASA + 0x1C8064) = 0x01;

    aml->Unprot(pGTASA + 0x1C8082, sizeof(char));
    *(char*)(pGTASA + 0x1C8082) = 0x01;
}