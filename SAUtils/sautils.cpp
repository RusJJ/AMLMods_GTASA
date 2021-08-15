#include <mod/amlmod.h>
#include <mod/logger.h>
#include <sautils.h>
#include <stdint.h>
#include <vector>

struct AdditionalSetting
{
    int nSettingId;
    eTypeOfSettings eType;
    const char* szName;
    OnSettingChangedFn fnOnValueChange;
    bool bIsSlider;
    int nInitVal;
    int nSavedVal;
    int nMaxVal;
};

std::vector<AdditionalSetting*> gMoreSettings;

int nNextSettingNum = MODS_SETTINGS_STARTING_FROM - 1;

typedef void* (*TextureDBGetTextureFn)(uintptr_t a1, uintptr_t a2);
typedef void (*AsciiToGxtCharFn)(const char*, unsigned short*);


/* Funcs */
AsciiToGxtCharFn fnAsciiToGxtChar;
typedef void* (*SettingsAddItemFn)(void* a1, uintptr_t a2);
/* Funcs */

/* GTASA Pointers */
extern uintptr_t pGTASA;
unsigned short* gxtErrorString;
unsigned char* aScreens;
SettingsAddItemFn fnSettingsAddItem;
/* GTASA Pointers */


DECL_HOOK(unsigned short*, GxtTextGet, void* self, const char* txt)
{
    static unsigned short gxtTxt[0x7F];
    unsigned short* ret = GxtTextGet(self, txt);
    if(ret == gxtErrorString)
    {
        fnAsciiToGxtChar(txt, gxtTxt);
        return gxtTxt;
    }
    return ret;
}

bool bPassRTDBtn = false;
uintptr_t pLatestRTDPointer = 0;
eTypeOfSettings nLatestSettingsOpened;
DECL_HOOK(void, SelectScreenAddItem, void* self, uintptr_t item)
{
    if(bPassRTDBtn && !strcmp(*(char**)(item + 4), "MOB_RTD"))
    {
        pLatestRTDPointer = item;
        return;
    }
    SelectScreenAddItem(self, item);
}
void AddSettingsToScreen(void* screen)
{
    auto vStart = gMoreSettings.begin();
    auto vEnd = gMoreSettings.end();
    while(vStart != vEnd)
    {
        if((*vStart)->eType == nLatestSettingsOpened)
        {
            uintptr_t menuItem = (uintptr_t)(new char[0x1Cu]);
            *(uintptr_t*)menuItem = pGTASA + 0x662848;
            *(const char**)(menuItem + 4) = (*vStart)->szName;
            *(int*)(menuItem + 8) = (*vStart)->nSettingId;
            *(int*)(menuItem + 12) = 0;
            *(int*)(menuItem + 16) = 0;
            SelectScreenAddItem(screen, menuItem);
        }
        ++vStart;
    }
    // Bring back "Reset To Defaults" button
    if(pLatestRTDPointer != 0) SelectScreenAddItem(screen, pLatestRTDPointer);
}
void SettingsScreenClosed()
{
    auto vStart = gMoreSettings.begin();
    auto vEnd = gMoreSettings.end();
    while(vStart != vEnd)
    {
        if((*vStart)->eType == nLatestSettingsOpened)
        {
            int nNewVal = sautils->ValueOfSettingsItem((*vStart)->nSettingId);
            if(nNewVal != (*vStart)->nSavedVal)
            {
                if((*vStart)->fnOnValueChange != nullptr) (*vStart)->fnOnValueChange((*vStart)->nSavedVal, nNewVal);
                (*vStart)->nSavedVal = nNewVal;
            }
        }
        ++vStart;
    }
}

DECL_HOOK(void*, NewScreen_Controls, void* self)
{
    pLatestRTDPointer = 0;
    bPassRTDBtn = true;
    void* ret = NewScreen_Controls(self);
    bPassRTDBtn = false;
    nLatestSettingsOpened = Controller;
    AddSettingsToScreen(self);
    return ret;
}

DECL_HOOK(void*, NewScreen_Game, void* self)
{
    pLatestRTDPointer = 0;
    bPassRTDBtn = true;
    void* ret = NewScreen_Game(self);
    bPassRTDBtn = false;
    nLatestSettingsOpened = Game;
    AddSettingsToScreen(self);
    return ret;
}

DECL_HOOK(void*, NewScreen_Display, void* self)
{
    pLatestRTDPointer = 0;
    bPassRTDBtn = true;
    void* ret = NewScreen_Display(self);
    bPassRTDBtn = false;
    nLatestSettingsOpened = Display;
    AddSettingsToScreen(self);
    return ret;
}

DECL_HOOK(void*, NewScreen_Audio, void* self)
{
    pLatestRTDPointer = 0;
    bPassRTDBtn = true;
    void* ret = NewScreen_Audio(self);
    bPassRTDBtn = false;
    nLatestSettingsOpened = Audio;
    AddSettingsToScreen(self);
    return ret;
}

DECL_HOOK(void*, NewScreen_Language, void* self)
{
    pLatestRTDPointer = 0;
    bPassRTDBtn = true;
    void* ret = NewScreen_Language(self);
    bPassRTDBtn = false;
    nLatestSettingsOpened = Language;
    AddSettingsToScreen(self);
    return ret;
}

DECL_HOOK(void, SelectScreenOnDestroy, void* self)
{
    SettingsScreenClosed();
    SelectScreenOnDestroy(self);
}

/* A new MobileSettings::settings[37*8*4] */
int pNewSettings[32 * 100];
void SAUtils::InitializeUtils()
{
    gxtErrorString = (unsigned short*)(pGTASA + 0xA01A90);
    aScreens = (unsigned char*)(pGTASA + 0x6AB480);

    aml->Unprot(pGTASA + 0x679A40, sizeof(void*));
    *(uintptr_t*)(pGTASA + 0x679A40) = (uintptr_t)pNewSettings;
    memcpy(pNewSettings, (int*)(pGTASA + 0x6E03F4), 1184);

    HOOKPLT(GxtTextGet, pGTASA + 0x66E78C);
    HOOKPLT(NewScreen_Controls, pGTASA + 0x675CD8);
    HOOKPLT(NewScreen_Game, pGTASA + 0x674310);
    HOOKPLT(NewScreen_Display, pGTASA + 0x675150);
    HOOKPLT(NewScreen_Audio, pGTASA + 0x66FBA4);
    HOOKPLT(NewScreen_Language, pGTASA + 0x675D90);
    HOOKPLT(SelectScreenAddItem, pGTASA + 0x674518);
    HOOKPLT(SelectScreenOnDestroy, pGTASA + 0x673FD8);

    fnAsciiToGxtChar = (AsciiToGxtCharFn)(pGTASA + 0x196430);
    fnSettingsAddItem = (SettingsAddItemFn)(pGTASA + 0x19C840);
}



/* Interface */
/* Interface */
/* Interface */

uintptr_t SAUtils::IsFLALoaded()
{
    return m_pHasFLA;
}

int SAUtils::AddSettingsItem(eTypeOfSettings typeOf, const char* name, int initVal, int minVal, int maxVal, OnSettingChangedFn fnOnValueChange, bool isSlider, void* switchesArray)
{
    ++nNextSettingNum;
    AdditionalSetting* pNew = new AdditionalSetting;
    pNew->nSettingId = nNextSettingNum;
    pNew->eType = typeOf;
    pNew->szName = name;
    pNew->fnOnValueChange = fnOnValueChange;
    pNew->bIsSlider = isSlider;
    pNew->nInitVal = (int)initVal;
    pNew->nSavedVal = (int)initVal;
    pNew->nMaxVal = maxVal;
    gMoreSettings.push_back(pNew);

    pNewSettings[8 * nNextSettingNum + 1] = (int)switchesArray; // Items of that setting
    pNewSettings[8 * nNextSettingNum + 2] = initVal; // Current value
    pNewSettings[8 * nNextSettingNum + 4] = minVal; // Min slider value (min is -2millions) OR min count of items (keep it 0 maybe, if u dont need others)
    pNewSettings[8 * nNextSettingNum + 5] = maxVal; // Max slider value (max is 2millions) OR max count-1 of items
    pNewSettings[8 * nNextSettingNum + 7] = isSlider?1:0; // Declare it as a slider (flags???)

    return nNextSettingNum;
}

int SAUtils::ValueOfSettingsItem(int settingId)
{
    if(settingId < MODS_SETTINGS_STARTING_FROM || settingId > nNextSettingNum) return 0;
    return pNewSettings[8 * settingId + 2];
}

static SAUtils sautilsLocal;
ISAUtils* sautils = &sautilsLocal;