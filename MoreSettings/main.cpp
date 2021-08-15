#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include <string>
#include "isautils.h"
ISAUtils* sautils = nullptr;

MYMODCFG(net.rusjj.gtasa.moresettings, GTA:SA More Settings, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0)
    ADD_DEPENDENCY_VER(net.rusjj.gtasa.utils, 1.0)
END_DEPLIST()

/* Saves */
uintptr_t pGTASA = 0;
ConfigEntry* pCfgDebugFPS;
ConfigEntry* pCfgFPS;
//ConfigEntry* pCfgFogReduction;

void DebugFPSChanged(int oldVal, int newVal)
{
    pCfgDebugFPS->SetBool(newVal==0?false:true);
    *(bool*)(pGTASA + 0x98F1AD) = pCfgDebugFPS->GetBool();
    cfg->Save();
}

char nFPSArray[] = 
{
    20,
    30,
    45,
    60,
    90,
    120,
    0,
};
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

const char* pYesNo[] = 
{
    "FEM_OFF",
    "FEM_ON",
};
const char* pFPSArray[] = 
{
    "20 FPS",
    "30 FPS",
    "45 FPS",
    "60 FPS",
    "90 FPS",
    "120 FPS",
    "Unknown FPS",
};
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
        pCfgDebugFPS = cfg->Bind("DebugFPS", (*(bool*)(pGTASA + 0x98F1AD)?"1":"0"), "Tweaks");
        *(bool*)(pGTASA + 0x98F1AD) = pCfgDebugFPS->GetBool();
        sautils->AddSettingsItem(Game, "Debug FPS", pCfgDebugFPS->GetInt(), 0, 1, DebugFPSChanged, false, (void*)pYesNo);

        int ifps = 0;
        for(;ifps < 6; ++ifps)
        {
            if(nFPSArray[ifps] == *(char*)(pGTASA + 0x5E4978))
                break;
        }
        if(ifps >= 6) ifps = -1;
        pCfgFPS = cfg->Bind("FPS", std::to_string(ifps).c_str(), "Tweaks");
        if(ifps > -1 && ifps < 6)
        {
            *(char*)(pGTASA + 0x5E4978) = nFPSArray[pCfgFPS->GetInt()];
            *(char*)(pGTASA + 0x5E4990) = nFPSArray[pCfgFPS->GetInt()];
        }
        sautils->AddSettingsItem(Game, "FPS", ifps!=-1?ifps:6, 0, 5, FPSChanged, false, (void*)pFPSArray);

        // Seems like it`s not working :(
        //char defValStringified[16];
        //sprintf(defValStringified, "%f", *(float*)(pGTASA + 0x41F300));
        //pCfgFogReduction = cfg->Bind("FogReduction", defValStringified, "Tweaks");
        //sautils->AddSettingsItem(Display, "Fog Reduction", (int)(pCfgFogReduction->GetFloat() * 10.0f), 0, 250, OnFogReductionChange, true);
    }
}