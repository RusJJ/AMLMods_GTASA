#include <mod/amlmod.h>

MYMOD(net.rusjj.forcedxt, ForceDXT, 1.0, RusJJ)

uintptr_t pGameLib = 0;
extern "C" void OnModPreLoad()
{
    pGameLib = aml->GetLib("libGTASA.so");
    if(pGameLib)
    {
        // TextureDatabase::LoadThumbs
        // ETC
        aml->Unprot(pGameLib + 0x1E880C, 20);
        *(char*)(pGameLib + 0x1E880C + 12) = 'd';
        *(char*)(pGameLib + 0x1E880C + 13) = 'x';
        *(char*)(pGameLib + 0x1E880C + 14) = 't';
        // PVR
        aml->Unprot(pGameLib + 0x1E8820, 20);
        *(char*)(pGameLib + 0x1E8820 + 12) = 'd';
        *(char*)(pGameLib + 0x1E8820 + 13) = 'x';
        *(char*)(pGameLib + 0x1E8820 + 14) = 't';

        // TextureDatabase::LoadDataOffsets
        // ETC
        aml->Unprot(pGameLib + 0x1E8C74, 16);
        *(char*)(pGameLib + 0x1E8C74 + 13) = 'd';
        *(char*)(pGameLib + 0x1E8C74 + 14) = 'x';
        *(char*)(pGameLib + 0x1E8C74 + 15) = 't';
        // PVR
        aml->Unprot(pGameLib + 0x1E8C84, 16);
        *(char*)(pGameLib + 0x1E8C84 + 13) = 'd';
        *(char*)(pGameLib + 0x1E8C84 + 14) = 'x';
        *(char*)(pGameLib + 0x1E8C84 + 15) = 't';
        return;
    }

    pGameLib = aml->GetLib("libGTAVC.so");
    if(pGameLib)
    {
        // TextureDatabase::LoadThumbs
        // PVR
        aml->Unprot(pGameLib + 0x354F74, 17);
        *(char*)(pGameLib + 0x354F74 + 10) = 'd';
        *(char*)(pGameLib + 0x354F74 + 11) = 'x';
        *(char*)(pGameLib + 0x354F74 + 12) = 't';
        // ETC
        aml->Unprot(pGameLib + 0x354F88, 17);
        *(char*)(pGameLib + 0x354F88 + 10) = 'd';
        *(char*)(pGameLib + 0x354F88 + 11) = 'x';
        *(char*)(pGameLib + 0x354F88 + 12) = 't';

        // TextureDatabase::LoadDataOffsets
        // PVR
        aml->Unprot(pGameLib + 0x354FBC, 13);
        *(char*)(pGameLib + 0x354FBC + 10) = 'd';
        *(char*)(pGameLib + 0x354FBC + 11) = 'x';
        *(char*)(pGameLib + 0x354FBC + 12) = 't';
        // ETC
        aml->Unprot(pGameLib + 0x354FCC, 17);
        *(char*)(pGameLib + 0x354FCC + 10) = 'd';
        *(char*)(pGameLib + 0x354FCC + 11) = 'x';
        *(char*)(pGameLib + 0x354FCC + 12) = 't';
        return;
    }
}