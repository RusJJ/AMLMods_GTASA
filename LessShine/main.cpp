#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

MYMODCFG(net.rusjj.gtasa.lessshine, GTA:SA Less Ped Shining, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGTASA = 0;
ConfigEntry* pShinessScale;

unsigned char* pDirectionalMult;
DECL_HOOK(bool, TimeCycleInit, void* self, bool a1)
{
    bool ret = TimeCycleInit(self, a1);
    /*for(int hour = 0; hour < 24; ++hour)
    {
        for(int weatherType = 0; weatherType < 0; ++weatherType)
        {
            ((char*)(nDirectionalMult + 23*hour))[weatherType] = (unsigned char)pShinessScale->GetInt();
        }
    }*/
    for(int i = 0; i < 184; ++i)
    {
        pDirectionalMult[i] = (unsigned char)pShinessScale->GetInt();
    }
    return ret;
}

extern "C" void OnModLoad()
{
    logger->SetTag("GTASA LessShine");
    pGTASA = aml->GetLib("libGTASA.so");

    pShinessScale = cfg->Bind("ShinessScale", "30");

    pDirectionalMult = (unsigned char*)(pGTASA + 0x968A18);
    HOOKPLT(TimeCycleInit, pGTASA + 0x67489C);
}