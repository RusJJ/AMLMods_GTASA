#include <string>
#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

MYMODCFG(net.rusjj.gtasa.moresettings, GTA:SA More Settings, 1.2, RusJJ)
NEEDGAME(com.rockstargames.gtasa)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0)
    ADD_DEPENDENCY_VER(net.rusjj.gtasa.utils, 1.1)
END_DEPLIST()

#include "isautils.h"
ISAUtils* sautils = nullptr;

/* Saves */
uintptr_t pGTASA = 0;
bool bForceNoCull = false;

/* Config */
ConfigEntry* pCfgDebugFPS;
ConfigEntry* pCfgFPS;
ConfigEntry* pCfgBackfaceCulling;
ConfigEntry* pCfgVehicleBackfaceCulling;
ConfigEntry* pCfgBreakableBackfaceCulling;
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
void BackfaceCullingChanged(int oldVal, int newVal)
{
    pCfgBackfaceCulling->SetInt(newVal);
    cfg->Save();
}
void VehicleBackfaceCullingChanged(int oldVal, int newVal)
{
    pCfgVehicleBackfaceCulling->SetInt(newVal);
    cfg->Save();
}
void BreakableBackfaceCullingChanged(int oldVal, int newVal)
{
    pCfgBreakableBackfaceCulling->SetInt(newVal);
    cfg->Save();
}
//void OnFogReductionChange(int oldVal, int newVal)
//{
//    pCfgFogReduction->SetFloat(0.1f * newVal);
//    *(float*)(pGTASA + 0x41F300) = pCfgFogReduction->GetFloat();
//    cfg->Save();
//}
DECL_HOOK(void, RwRenderStateSet, int state, int val)
{
    if(state == 20 && (bForceNoCull || pCfgBackfaceCulling->GetBool()))
    {
        logger->Info("RwRenderStateSet(20, 1);");
        RwRenderStateSet(20, 1);
        return;
    }
    logger->Info("RwRenderStateSet(%d, %d);", state, val);
    RwRenderStateSet(state, val);
}
DECL_HOOK(void, EntityRender, uintptr_t self)
{
    static short model_id;
    model_id = *(short*)(self + 38);
    if(model_id >= 400 && model_id < 615 && pCfgVehicleBackfaceCulling->GetBool())
    {
        logger->Info("VehicleRender");
        bForceNoCull = true;
        //RwRenderStateSet(20, 1);
        EntityRender(self);
        bForceNoCull = false;
        //HookOf_RwRenderStateSet(20, 2);
        return;
    }
    EntityRender(self);
}
DECL_HOOK(void, BreakableRender, void* self)
{
    if(pCfgBreakableBackfaceCulling->GetBool())
    {
        RwRenderStateSet(20, 1);
        BreakableRender(self);
        HookOf_RwRenderStateSet(20, 2);
        return;
    }
    BreakableRender(self);
}

extern "C" void OnModLoad()
{
    logger->SetTag("GTASA More Settings");
    pGTASA = aml->GetLib("libGTASA.so");
    
    aml->Unprot(pGTASA + 0x98F1AD, sizeof(bool)); // Debug FPS
    aml->Unprot(pGTASA + 0x5E4978, sizeof(char)); aml->Unprot(pGTASA + 0x5E4990, sizeof(char)); // FPS
    //aml->Unprot(pGTASA + 0x41F300, sizeof(float));

    sautils = (ISAUtils*)GetInterface("SAUtils");
    if(sautils != nullptr)
    {
        pCfgDebugFPS = cfg->Bind("DebugFPS", *(bool*)(pGTASA + 0x98F1AD), "Tweaks");
        *(bool*)(pGTASA + 0x98F1AD) = pCfgDebugFPS->GetBool();
        sautils->AddClickableItem(Game, "Debug FPS", pCfgDebugFPS->GetInt(), 0, sizeofA(pYesNo)-1, pYesNo, DebugFPSChanged);

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
        sautils->AddClickableItem(Game, "FPS", pCfgFPS->GetInt(), 0, sizeofA(pFPSArray)-1, pFPSArray, FPSChanged);
        
        // Seems like it`s not working :(
        //char defValStringified[16];
        //sprintf(defValStringified, "%f", *(float*)(pGTASA + 0x41F300));
        //pCfgFogReduction = cfg->Bind("FogReduction", defValStringified, "Tweaks");
        //sautils->AddSettingsItem(Display, "Fog Reduction", (int)(pCfgFogReduction->GetFloat() * 10.0f), 0, 250, OnFogReductionChange, true);

        // Backface Culling
        pCfgBackfaceCulling = cfg->Bind("DisableBackfaceCulling", false, "Tweaks");
        sautils->AddClickableItem(Display, "Disable Backface Culling", pCfgBackfaceCulling->GetBool(), 0, sizeofA(pYesNo)-1, pYesNo, BackfaceCullingChanged);
        HOOKPLT(RwRenderStateSet, pGTASA + 0x6711B8);

        // Vehicle Backface Culling
        //pCfgVehicleBackfaceCulling = cfg->Bind("VehicleDisableBackfaceCulling", false, "Tweaks");
        //sautils->AddSettingsItem(Display, "Disable Backface Culling for Vehicle", pCfgVehicleBackfaceCulling->GetBool(), 0, sizeofA(pYesNo)-1, VehicleBackfaceCullingChanged, false, (void*)pYesNo);
        //HOOKPLT(EntityRender, pGTASA + 0x66F764);

        // Breakable Backface Culling
        //pCfgBreakableBackfaceCulling = cfg->Bind("BreakableDisableBackfaceCulling", false, "Tweaks");
        //sautils->AddSettingsItem(Display, "Disable Backface Culling for Breakable", pCfgBreakableBackfaceCulling->GetBool(), 0, sizeofA(pYesNo)-1, BreakableBackfaceCullingChanged, false, (void*)pYesNo);
        //HOOKPLT(BreakableRender, pGTASA + 0x6723CC);
    }
}