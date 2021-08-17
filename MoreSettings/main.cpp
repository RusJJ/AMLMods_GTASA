#include <string>
#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

MYMODCFG(net.rusjj.gtasa.moresettings, GTA:SA More Settings, 1.1, RusJJ)
NEEDGAME(com.rockstargames.gtasa)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0)
    ADD_DEPENDENCY_VER(net.rusjj.gtasa.utils, 1.0)
END_DEPLIST()

#include "isautils.h"
ISAUtils* sautils = nullptr;

/* Saves */
uintptr_t pGTASA = 0;
ConfigEntry* pCfgDebugFPS;
ConfigEntry* pCfgFPS;
//ConfigEntry* pCfgFogReduction;

const char* pYesNo[] = 
{
    "FEM_OFF",
    "FEM_ON",
};
char nFPSArray[] = 
{
    20, 30, 45, 60, 90, 120,
};
const char* pFPSArray[] = 
{
    "20 FPS", "30 FPS", "45 FPS", "60 FPS", "90 FPS", "120 FPS",
};
static_assert(sizeofA(nFPSArray) == sizeofA(pFPSArray), "nFPSArray`s and pFPSArray`s sizes are NOT equal!");

void DebugFPSChanged(int oldVal, int newVal)
{
    pCfgDebugFPS->SetBool(newVal==0?false:true);
    *(bool*)(pGTASA + 0x98F1AD) = pCfgDebugFPS->GetBool();
    cfg->Save();
}
void FPSChanged(int oldVal, int newVal)
{
    pCfgFPS->SetInt(newVal);
    *(char*)(pGTASA + 0x5E4978) = nFPSArray[newVal];
    *(char*)(pGTASA + 0x5E4990) = nFPSArray[newVal];
    cfg->Save();
}

//void OnFogReductionChange(int oldVal, int newVal)
//{
//    pCfgFogReduction->SetFloat(0.1f * newVal);
//    *(float*)(pGTASA + 0x41F300) = pCfgFogReduction->GetFloat();
//    cfg->Save();
//}

extern "C" void OnModLoad()
{
    logger->SetTag("GTASA More Settings");
    pGTASA = aml->GetLib("libGTASA.so");
    
    aml->Unprot(pGTASA + 0x98F1AD, sizeof(bool));
    aml->Unprot(pGTASA + 0x5E4978, sizeof(char)); aml->Unprot(pGTASA + 0x5E4990, sizeof(char));
    //aml->Unprot(pGTASA + 0x41F300, sizeof(float));

    sautils = (ISAUtils*)GetInterface("SAUtils");
    if(sautils != nullptr)
    {
        pCfgDebugFPS = cfg->Bind("DebugFPS", *(bool*)(pGTASA + 0x98F1AD), "Tweaks");
        *(bool*)(pGTASA + 0x98F1AD) = pCfgDebugFPS->GetBool();
        sautils->AddSettingsItem(Game, "Debug FPS", pCfgDebugFPS->GetInt(), 0, sizeofA(pYesNo)-1, DebugFPSChanged, false, (void*)pYesNo);

        // Lookup for FPS
        pCfgFPS = cfg->Bind("FPS", 1, "Tweaks"); // Def is 30 FPS (do not detected it automatically, useless)
        int nFPS = pCfgFPS->GetInt();
        if(nFPS < 0 || nFPS >= sizeofA(nFPSArray))
        {
            nFPS = 1;
            pCfgFPS->SetInt(1);
            cfg->Save();
        }
        *(char*)(pGTASA + 0x5E4978) = nFPSArray[nFPS];
        *(char*)(pGTASA + 0x5E4990) = nFPSArray[nFPS];
        sautils->AddSettingsItem(Game, "FPS", pCfgFPS->GetInt(), 0, sizeofA(pFPSArray)-1, FPSChanged, false, (void*)pFPSArray);

        // Seems like it`s not working :(
        //char defValStringified[16];
        //sprintf(defValStringified, "%f", *(float*)(pGTASA + 0x41F300));
        //pCfgFogReduction = cfg->Bind("FogReduction", defValStringified, "Tweaks");
        //sautils->AddSettingsItem(Display, "Fog Reduction", (int)(pCfgFogReduction->GetFloat() * 10.0f), 0, 250, OnFogReductionChange, true);
    }
}