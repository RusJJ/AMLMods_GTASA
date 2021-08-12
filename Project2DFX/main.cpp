#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

class CEntity;

MYMODCFG(net.rusjj.gtasa.2dfx, Project 2DFX, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t       pGTASA = 0;
int*            ms_modelInfoPtrs;
int*            nActiveInterior;
float*          flWeatherFoggyness;
void*           TheCamera; // CCamera*

ConfigEntry*    pCfgLamppostCoronaFarClip;

ConfigEntry*    pCfgVegetationDrawDistance;
ConfigEntry*    pCfgLODDrawDistance;
ConfigEntry*    pCfgGenericDrawDistance;
ConfigEntry*    pCfgNormalDrawDistance;
ConfigEntry*    pCfgTimedDrawDistance;

float           fCoronaDrawDistance;
float           fStaticShadowsDrawDistance;
float           fMaxDrawDistanceForNormalObjects;
float           fSearchlightEffectVisibilityFactor;
const char*     szCustomCoronaTexturePath;

char(*GetIsTimeInRange)(char hourA, char hourB);
float(*FindGroundZFor3DCoord)(float x, float y, float z, BOOL *pCollisionResult, CEntity **pGroundObject);

DECL_HOOK(int, FileLoaderLoadObject, const char* a1)
{
    int nModelId = FileLoaderLoadObject(a1);
    float* drawDist = ((float*)ms_modelInfoPtrs[nModelId] + 12);

    if(pCfgVegetationDrawDistance->GetFloat() > 0)
    {
        if (nModelId >= 615 && nModelId <= 792 && *drawDist <= 300.0f)
        {
            if(pCfgVegetationDrawDistance->GetFloat() <= 10.0f)
                *drawDist *= pCfgVegetationDrawDistance->GetFloat();
            else
                *drawDist = pCfgVegetationDrawDistance->GetFloat();

            if (*drawDist > fMaxDrawDistanceForNormalObjects)
            {
                fMaxDrawDistanceForNormalObjects = *drawDist;
                //*(int*)(pGTASA + 0x858FD8) = fMaxDrawDistanceForNormalObjects;
            }
            return nModelId;
        }
    }

    if(*drawDist > 300.0f)
    {
        if (pCfgLODDrawDistance->GetFloat() > 0)
        {
            if(pCfgLODDrawDistance->GetFloat() <= 10.0f)
                *drawDist *= pCfgLODDrawDistance->GetFloat();
            else
                *drawDist = pCfgLODDrawDistance->GetFloat();
        }
    }
    else
    {
        if(pCfgGenericDrawDistance->GetFloat() > 0 && nModelId >= 615 && nModelId <= 1572)
        {
            if(pCfgGenericDrawDistance->GetFloat() <= 10.0f)
                *drawDist *= pCfgGenericDrawDistance->GetFloat();
            else
                *drawDist = pCfgGenericDrawDistance->GetFloat();
        }
        else
        {
            if (pCfgNormalDrawDistance->GetFloat() > 0)
            {
                if(pCfgNormalDrawDistance->GetFloat() <= 10.0f)
                    *drawDist *= pCfgNormalDrawDistance->GetFloat();
                else
                    *drawDist = pCfgNormalDrawDistance->GetFloat();
            }
        }
        if (*drawDist > fMaxDrawDistanceForNormalObjects)
        {
            fMaxDrawDistanceForNormalObjects = *drawDist;
            //*(int*)(pGTASA + 0x858FD8) = fMaxDrawDistanceForNormalObjects;
        }
    }

    return nModelId;
}

DECL_HOOK(int, FileLoaderLoadTimeObject, const char* a1)
{
    int nModelId = FileLoaderLoadTimeObject(a1);
    float* drawDist = ((float*)ms_modelInfoPtrs[nModelId] + 12);

    if (pCfgTimedDrawDistance->GetFloat() > 0)
    {
        if(pCfgTimedDrawDistance->GetFloat() <= 10.0f)
            *drawDist *= pCfgTimedDrawDistance->GetFloat();
        else
            *drawDist = pCfgTimedDrawDistance->GetFloat();
    }

    return nModelId;
}

DECL_HOOK(void*, RegisterCorona, void* self, unsigned int a2, void* a3, float a4, float a5, unsigned char a6, unsigned char a7, const void* a8, float distance, float a10, float a11, unsigned char a12, float a13, unsigned char a14, float a15, float a16, bool a17, float a18, bool a19, float a20, bool a21, bool a22)
{
    if(a12 == (unsigned char)1 && distance > 49.9f && distance < 50.1f)
    {
        return RegisterCorona(self, a2, a3, a4, a5, a6, a7, a8, distance, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
    }
    return RegisterCorona(self, a2, a3, a4, a5, a6, a7, a8, distance, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
}

DECL_HOOK(void*, RegisterCorona2, int a1, void* self, int a3, int a4, int a5, int alpha, int a7, float a8, float a9, int a10, int a11, int a12, int a13, int a14, float a15, int a16, float a17, int a18, float a19, int a20, int a21)
{
    if(a14 == 0 && a15 == 0.0f && a17 == 0.8f && a18 == 0 && a19 == 15.0f)
    {
        return RegisterCorona2(a1, self, a3, a4, a5, alpha, a7, a8, pCfgLamppostCoronaFarClip->GetFloat(), a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
    }
    return RegisterCorona2(a1, self, a3, a4, a5, alpha, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
}

DECL_HOOK(void*, StoreStaticShadow, unsigned int id, unsigned char type, void* texture, void* posn, float frontX, float frontY, float sideX, float sideY, short intensity, unsigned char r, unsigned char g, unsigned char b, float zDistance, float scale, float drawDistance)
{
    if(fStaticShadowsDrawDistance <= 10.0f)
        return StoreStaticShadow(id, type, texture, posn, frontX, frontY, sideX, sideY, intensity, r, g, b, zDistance, scale, fStaticShadowsDrawDistance * drawDistance);
    return StoreStaticShadow(id, type, texture, posn, frontX, frontY, sideX, sideY, intensity, r, g, b, zDistance, scale, fStaticShadowsDrawDistance);
}

DECL_HOOK(void*, MovingThingsRender)
{
    
    return MovingThingsRender();
}

extern "C" void OnModLoad()
{
    logger->SetTag("Project 2DFX");
    pGTASA = aml->GetLib("libGTASA.so");

    pCfgLamppostCoronaFarClip = cfg->Bind("LamppostCoronaFarClip", "3000.0", "Lighting");
    fCoronaDrawDistance = cfg->Bind("CoronaDistance", "550.0", "Lighting")->GetFloat();
    //szCustomCoronaTexturePath = cfg->Bind("CustomCoronaTexturePath", "corona.png", "Lighting")->GetString();

    pCfgVegetationDrawDistance = cfg->Bind("VegetationDrawDistance", "0.0", "DrawDistance");
    pCfgLODDrawDistance = cfg->Bind("LODDrawDistance", "0.0", "DrawDistance");
    pCfgGenericDrawDistance = cfg->Bind("GenericDrawDistance", "0.0", "DrawDistance");
    pCfgNormalDrawDistance = cfg->Bind("NormalDrawDistance", "0.0", "DrawDistance");
    pCfgTimedDrawDistance = cfg->Bind("TimedDrawDistance", "0.0", "DrawDistance");

    fStaticShadowsDrawDistance = cfg->Bind("StaticShadowsDrawDistance", "0.0", "StaticShadows")->GetFloat();

    fMaxDrawDistanceForNormalObjects = cfg->Bind("MaxDrawDistanceForNormalObjects", "300.0", "IDETweaker")->GetFloat();

    ms_modelInfoPtrs = (int*)(pGTASA + 0x91DCB8);
    nActiveInterior = (int*)(pGTASA + 0x95957C);
    flWeatherFoggyness = (float*)(pGTASA + 0xA7D14C);
    TheCamera = (void*)(pGTASA + 0x951FA8);

    GetIsTimeInRange = (char(*)(char, char)) (pGTASA + 0x1956A0);
    FindGroundZFor3DCoord = (float(*)(float, float, float, BOOL *, CEntity **)) (pGTASA + 0x67022C);

    HOOKPLT(FileLoaderLoadObject, pGTASA + 0x6714F8);
    HOOKPLT(FileLoaderLoadTimeObject, pGTASA + 0x67062C);

    if(fCoronaDrawDistance > 0.0f)
        HOOKPLT(RegisterCorona, pGTASA + 0x66E5B0);
    if(pCfgLamppostCoronaFarClip->GetInt() > 0)
        HOOKPLT(RegisterCorona2, pGTASA + 0x675C30);
    if(fStaticShadowsDrawDistance > 0)
        HOOKPLT(StoreStaticShadow, pGTASA + 0x66EF64);
    /* Post Glass::Render, Pre RenderLastObjects */
    HOOKPLT(MovingThingsRender, pGTASA + 0x673094);
}