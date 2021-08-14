#include <isautils.h>

#define MODS_SETTING_STARTING_FROM  37

class SAUtils : public ISAUtils
{
public:
    void InitializeUtils();

    uintptr_t IsFLALoaded();
    int AddSettingsItem(eTypeOfSettings typeOf, const char* name, int initVal = 0, int minVal = 0, int maxVal = 0, OnSettingChangedFn fnOnValueChange = nullptr, bool isSlider = false, void* switchesArray = nullptr);
    int ValueOfSettingsItem(int settingId);

public:
    uintptr_t m_pHasFLA;
};
extern ISAUtils* sautils;