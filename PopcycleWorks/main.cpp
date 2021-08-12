#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

MYMODCFG(net.rusjj.gtasa.popcycle, Popcycle Works, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGTASA = 0;

char* CPopCycle_nPercTypeGroup; // 0x21C0
unsigned char* CPopCycle_nMaxNumPeds;
unsigned char* CPopCycle_nMaxNumCars;
unsigned char* CPopCycle_nPercDealers;
unsigned char* CPopCycle_nPercGang;
unsigned char* CPopCycle_nPercCops;
unsigned char* CPopCycle_nPercOther;

ConfigEntry* pCfgScaleMaxNumPeds;
ConfigEntry* pCfgScaleMaxNumCars;
ConfigEntry* pCfgScalePercDealers;
ConfigEntry* pCfgScalePercGang;
ConfigEntry* pCfgScalePercCops;
ConfigEntry* pCfgScalePercOther;

ConfigEntry* pCfgMaxNumPeds;
ConfigEntry* pCfgMaxNumCars;
ConfigEntry* pCfgPercDealers;
ConfigEntry* pCfgPercGang;
ConfigEntry* pCfgPercCops;
ConfigEntry* pCfgPercOther;

DECL_HOOK(void*, PopcycleInit, void* self, const char* a1)
{
    void* ret = PopcycleInit(self, a1);

    char* v11, *v14;
    int v8, v10, v13;
    for(int v59 = 0; v59 < 20; ++v59)
    {
        for(int v60 = 0; v60 < 2; ++v60)
        {
            for(int v69 = 0; v69 < 11; ++v69)
            {
                v8 = 5 * v69;
                v10 = 5 * v60;
                v13 = 9 * v59;
                
                if(pCfgScaleMaxNumPeds->GetFloat() > 0.0f)
                    CPopCycle_nMaxNumPeds[20 * v60 + 8 * v8 + v59] *= pCfgScaleMaxNumPeds->GetFloat();
                else
                    CPopCycle_nMaxNumPeds[20 * v60 + 8 * v8 + v59] = (unsigned char)pCfgMaxNumPeds->GetInt();

                if(pCfgScaleMaxNumCars->GetFloat() > 0.0f)
                    CPopCycle_nMaxNumCars[8 * v8 + 4 * v10 + v59] *= pCfgScaleMaxNumCars->GetFloat();
                else
                    CPopCycle_nMaxNumCars[8 * v8 + 4 * v10 + v59] = (unsigned char)pCfgMaxNumCars->GetInt();

                if(pCfgScalePercDealers->GetFloat() > 0.0f)
                    CPopCycle_nPercDealers[8 * v8 + 4 * v10 + v59] *= pCfgScalePercDealers->GetFloat();
                else
                    CPopCycle_nPercDealers[8 * v8 + 4 * v10 + v59] = (unsigned char)pCfgPercDealers->GetInt();

                if(pCfgScalePercGang->GetFloat() > 0.0f)
                    CPopCycle_nPercGang[8 * v8 + 4 * v10 + v59] *= pCfgScalePercGang->GetFloat();
                else
                    CPopCycle_nPercGang[8 * v8 + 4 * v10 + v59] = (unsigned char)pCfgPercGang->GetInt();

                if(pCfgScalePercCops->GetFloat() > 0.0f)
                    CPopCycle_nPercCops[8 * v8 + 4 * v10 + v59] *= pCfgScalePercCops->GetFloat();
                else
                    CPopCycle_nPercCops[8 * v8 + 4 * v10 + v59] = (unsigned char)pCfgPercCops->GetInt();

                if(pCfgScalePercOther->GetFloat() > 0.0f)
                    CPopCycle_nPercOther[20 * v60 + 8 * v8 + v59] *= pCfgScalePercOther->GetFloat();
                else
                    CPopCycle_nPercOther[20 * v60 + 8 * v8 + v59] = (unsigned char)pCfgPercOther->GetInt();

                /* CurrentZoneType = (CPopCycle::m_pCurrZoneInfo + 15) & 0x1F */
                /* Spawn chances for a current [720 * CurrentTimeIndex + 360 * CurrentTimeOfWeek + 18 * CurrentZoneType] */
                //v11 = CPopCycle_nPercTypeGroup + 720 * v69 + 360 * v60;
                //v14 = &v11[18 * v59];

                //v11[2 * v13] *= 2; // Workers
                //v14[1] *= 2; // Business
                //v14[2] *= 2; // Clubbers
                //v14[3] *= 2; // Farmers
                //v14[4] *= 2; // BeachFolk
                //v14[5] *= 2; // Parkfolk
                //v14[6] *= 2; // Casual_Rich
                //v14[7] *= 2; // Casual_Average
                //v14[8] *= 2; // Casual_Poor
                //v14[9] *= 2; // Prozzies
                //v14[10] *= 2; // Criminals
                //v14[11] *= 2; // Golfers
                //v14[12] *= 2; // Servants
                //v14[13] *= 2; // Aircrew
                //v14[14] *= 2; // Entertainers
                //v14[15] *= 2; // oot_fact (???)
                //v14[16] *= 2; // Desertfolk
                //v14[17] *= 2; // Aircrew_runway
            }
        }
    }

    return ret;
}

extern "C" void OnModLoad() // A place for interfaces registering
{
    logger->SetTag("Popcycle Works");
    pGTASA = aml->GetLib("libGTASA.so");

    cfg->Bind("ScaleOrOverride", "Override wont work on scaling", "A Info");

    pCfgScaleMaxNumPeds = cfg->Bind("MaxNumPeds", "0.0", "Scale");
    pCfgScaleMaxNumCars = cfg->Bind("MaxNumCars", "0.0", "Scale");
    pCfgScalePercDealers = cfg->Bind("PercDealers", "0.0", "Scale");
    pCfgScalePercGang = cfg->Bind("PercGang", "0.0", "Scale");
    pCfgScalePercCops = cfg->Bind("PercCops", "0.0", "Scale");
    pCfgScalePercOther = cfg->Bind("PercOther", "0.0", "Scale");

    pCfgMaxNumPeds = cfg->Bind("MaxNumPeds", "-1", "OverrideNumber");
    if(pCfgMaxNumPeds->GetInt() > 255) pCfgMaxNumPeds->SetInt(255);
    pCfgMaxNumCars = cfg->Bind("MaxNumCars", "-1", "OverrideNumber");
    if(pCfgMaxNumCars->GetInt() > 255) pCfgMaxNumCars->SetInt(255);
    pCfgPercDealers = cfg->Bind("PercDealers", "-1", "OverrideNumber");
    if(pCfgPercDealers->GetInt() > 255) pCfgPercDealers->SetInt(255);
    pCfgPercGang = cfg->Bind("PercGang", "-1", "OverrideNumber");
    if(pCfgPercGang->GetInt() > 255) pCfgPercGang->SetInt(255);
    pCfgPercCops = cfg->Bind("PercCops", "-1", "OverrideNumber");
    if(pCfgPercCops->GetInt() > 255) pCfgPercCops->SetInt(255);
    pCfgPercOther = cfg->Bind("PercOther", "-1", "OverrideNumber");
    if(pCfgPercOther->GetInt() > 255) pCfgPercOther->SetInt(255);

    cfg->Save();

    CPopCycle_nPercTypeGroup = (char*)(pGTASA + 0x9F1664);
    CPopCycle_nMaxNumPeds = (unsigned char*)(pGTASA + 0x9F0B24);
    CPopCycle_nMaxNumCars = (unsigned char*)(pGTASA + 0x9F0D04);
    CPopCycle_nPercDealers = (unsigned char*)(pGTASA + 0x9F0EE4);
    CPopCycle_nPercGang = (unsigned char*)(pGTASA + 0x9F10C4);
    CPopCycle_nPercCops = (unsigned char*)(pGTASA + 0x9F12A4);
    CPopCycle_nPercOther = (unsigned char*)(pGTASA + 0x9F1484);
    
    if( pCfgScaleMaxNumPeds->GetFloat() > 0.0f || pCfgMaxNumPeds->GetInt() >= 0 || 
        pCfgScaleMaxNumCars->GetFloat() > 0.0f || pCfgMaxNumCars->GetInt() >= 0 || 
        pCfgScalePercDealers->GetFloat() > 0.0f || pCfgPercDealers->GetInt() >= 0 || 
        pCfgScalePercGang->GetFloat() > 0.0f || pCfgPercGang->GetInt() >= 0 || 
        pCfgScalePercCops->GetFloat() > 0.0f || pCfgPercCops->GetInt() >= 0 || 
        pCfgScalePercOther->GetFloat() > 0.0f || pCfgPercOther->GetInt() >= 0
    )
        HOOKPLT(PopcycleInit, pGTASA + 0x6734CC);
}