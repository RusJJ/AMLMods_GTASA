#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#define THUMB_ADDRESS(_address)   ((_address) | 1)

MYMODCFG(net.rusjj.gtasa.drawdistance, GTA:SA Draw Distance, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0)
END_DEPLIST()

/* Saves */
uintptr_t pGTASA = 0;
void* maincamera; // RwCamera*
unsigned int nStreamingMemoryOverride;
unsigned int nWaterBlocksToRender = 0;
uintptr_t pointerWaterBlocksToRender = 0;
float fAspectRatioScaler = 0.0f;

/* Configs */
ConfigEntry* pNearClipOverride;
ConfigEntry* pDrawDistanceOverride;
ConfigEntry* pStreamingDistanceScale;

/* Lib Pointers */
int* streamingMemoryAvailable;
float* lodDistScale;

/* Realloc */
int16_t* pBlocksToBeRenderedOutsideWorldX;
int16_t* pBlocksToBeRenderedOutsideWorldY;

DECL_HOOK(void*, RwCameraSetNearClipPlane, void* self, float a1)
{
    if(self == maincamera && pNearClipOverride->GetFloat() > 0.0f)
    {
        return RwCameraSetNearClipPlane(self, pNearClipOverride->GetFloat());
    }
    return RwCameraSetNearClipPlane(self, a1);
}
DECL_HOOK(void*, RwCameraSetFarClipPlane, void* self, float a1)
{
    if(self == maincamera && pDrawDistanceOverride->GetInt() >= 200)
    {
        return RwCameraSetFarClipPlane(self, pDrawDistanceOverride->GetFloat());
    }
    return RwCameraSetFarClipPlane(self, a1);
}

DECL_HOOK(void*, CameraCreate, void* a1, void* a2, int a3)
{
    maincamera = CameraCreate(a1, a2, a3);
    return maincamera;
}

DECL_HOOK(void*, CameraProcess, uintptr_t self)
{
    void* ret = CameraProcess(self);

    *(float*)(self + 236) *= fAspectRatioScaler;
    *(float*)(self + 240) *= fAspectRatioScaler;

    return ret;
}

DECL_HOOK(void*, CStreamingUpdate, void* self)
{
    logger->Info("unk_952098: %f", *(float*)(pGTASA + 0x952098));
    *(float*)(pGTASA + 0x952098) = 1.5f;
    if(*streamingMemoryAvailable < nStreamingMemoryOverride)
        *streamingMemoryAvailable = nStreamingMemoryOverride;
    return CStreamingUpdate(self);
}


TARGET_THUMB ASM_NAKED void jmp_HitBlock()
{
    __asm(
    ".hidden nWaterBlocksToRender\n"
    ".hidden pointerWaterBlocksToRender\n"
    ".thumb\n"
        "PUSH {R1}\n" // Backup R1
        "LDR R1, =(nWaterBlocksToRender - 100001f - 2*(100002f-100001f))\n" // Copy contents of variable to R0
        "100001:\nADD R1, PC\n100002:\n"
        "LDR R1, [R1]\n"
        
        "CMP R0, R1\n" // Compare R0=nWaterBlocksToRender with R1=m_NumBlocksOutsideWorldToBeRendered
        "POP {R1}\n" // Bring back R1

		"IT GT\n"
		"POPGT {R4,R5,R7,PC}\n"
		"LDR R1, =(0x6777C4 - 0x59869C)\n"

        "PUSH {R0,R1}\n"
        "LDR R0, =(pointerWaterBlocksToRender - 100001f - 2*(100002f-100001f))\n" // Copy contents of variable to R0
        "100001:\nADD R0, PC\n100002:\n"
        "LDR R0, [R0]\n"
        "STR R0, [SP, #4]\n"
        "POP {R0,PC}\n"
    );
} 

void CodeRedirect(uintptr_t address, uintptr_t newAddress, bool isThumb)
{
    if(isThumb)
    {
        char code[12];
        unsigned int sizeOfData = 0;

        if (address % 4 == 0)
        {
            *(uint32_t*)(code + 0) = 0xF000F8DF;
            *(const void**)(code + 4) = (const void*)newAddress;
            sizeOfData = 8;
        }
        else
        {
            *(uint32_t*)(code + 0) = 0xBF00;
            *(uint32_t*)(code + 2) = 0xF000F8DF;
            *(const void**)(code + 6) = (const void*)newAddress;
            sizeOfData = 10;
        }
        aml->Write(address, (uintptr_t)code, sizeOfData);
        return;
    }

	char code[8];
	*(uint32_t*)(code + 0) = 0xE51FF004;
	*(const void**)(code + 4) = (const void*)newAddress;
    aml->Write(address, (uintptr_t)code, sizeof(code));
}

typedef float (*RetFloatFn)();
extern "C" void OnModLoad()
{
    logger->SetTag("GTASA Draw Distance");
    pGTASA = aml->GetLib("libGTASA.so");

    pNearClipOverride = cfg->Bind("NearClip", "0.1");
    pDrawDistanceOverride = cfg->Bind("DrawDistance", "1200.0");
    nStreamingMemoryOverride = cfg->Bind("PreferredStreamingMemMB", "1024")->GetInt() * 1024 * 1024;
    nWaterBlocksToRender = (unsigned int)cfg->Bind("WaterBlocksToRender", "384")->GetInt();
    pStreamingDistanceScale = cfg->Bind("StreamingDistanceScale", "1.0");

    float fRealAspectRatio = ((RetFloatFn)(pGTASA + 0x18E984))();
    if(fRealAspectRatio < 1.0f) fRealAspectRatio = 1.0f / fRealAspectRatio;
    fAspectRatioScaler = fRealAspectRatio * 0.75 * pStreamingDistanceScale->GetFloat(); // AspectRatio / 4:3
    streamingMemoryAvailable = (int*)(pGTASA + 0x685FA0);

    if(nWaterBlocksToRender > 1)
    {
        if(nWaterBlocksToRender >= 69)
        {
            pBlocksToBeRenderedOutsideWorldX = new int16_t[nWaterBlocksToRender];
            aml->Write(pGTASA + 0x6777C4, (uintptr_t)&pBlocksToBeRenderedOutsideWorldX, sizeof(void*));

            pBlocksToBeRenderedOutsideWorldY = new int16_t[nWaterBlocksToRender];
            aml->Write(pGTASA + 0xA1C05C, (uintptr_t)&pBlocksToBeRenderedOutsideWorldY, sizeof(void*));
        }
        --nWaterBlocksToRender;
        pointerWaterBlocksToRender = (pGTASA + 0x598694) | 1;
        CodeRedirect(pGTASA + 0x59868C, (uintptr_t)&jmp_HitBlock, true);
    }

    HOOKPLT(RwCameraSetNearClipPlane, pGTASA + 0x670C9C);
    HOOKPLT(RwCameraSetFarClipPlane, pGTASA + 0x6740CC);

    HOOKPLT(CameraCreate, pGTASA + 0x675174);
    HOOKPLT(CameraProcess, pGTASA + 0x6717BC);

    HOOKPLT(CStreamingUpdate, pGTASA + 0x673898);

    if(cfg->Bind("MoreOftenPopulationUpdate", "1")->GetBool())
    {
        aml->Unprot(pGTASA + 0x3F40C0, sizeof(char));
        *(char*)(pGTASA + 0x3F40C0) = 2;
    }
    
    // Do not delete vehicles behind the player camera
    bool bRemoveCarsBehind = cfg->Bind("DontRemoveVehicleBehindCamera", "1")->GetBool();
    if(bRemoveCarsBehind)
    {
        //aml->PlaceJMP(pGTASA + 0x2EC660, pGTASA + 0x2EC6E8);
        aml->PlaceJMP(pGTASA + 0x2EC660, pGTASA + 0x2EC6D6);
    }
    
    // Do not delete peds behind the player camera
    if(cfg->Bind("DontRemovePedBehindCamera", "1")->GetBool())
    {
        //aml->PlaceJMP(pGTASA + 0x2EC660, pGTASA + 0x2EC6E8);
    }

    if(cfg->Bind("SpawnVehiclesInFrontOfPlayer", "1")->GetBool())
    {
        if(bRemoveCarsBehind) // They are already spawning!
        {
            //aml->Unprot(pGTASA + 0x2E866F, sizeof(char));
            //*(char*)(pGTASA + 0x2E866F) = 0xDB;
        }
        else
        {
            aml->PlaceJMP(pGTASA + 0x2E864E, pGTASA + 0x2E8670);
        }
    }
    else
    {
        if(bRemoveCarsBehind) // Huh?
        {
            aml->Unprot(pGTASA + 0x2E866F, sizeof(char));
            *(char*)(pGTASA + 0x2E866F) = 0xDB;
        }
    }
}