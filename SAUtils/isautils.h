#include <stdint.h>

typedef void (*OnSettingChangedFn)(int nOldValue, int nNewValue);

enum eTypeOfSettings : unsigned char
{
    Controller = 0,
    Game = 1,
    Display = 2,
    Audio = 3,
    Language = 4,
};

class ISAUtils
{
public:
  /* Functions below added in 1.0.0.0 */
    virtual uintptr_t IsFLALoaded() = 0;
    // switchesArray is an array of items of clickable item (isSlider = false)
    virtual int AddSettingsItem(eTypeOfSettings typeOf, const char* name, int initVal = 0, int minVal = 0, int maxVal = 0, OnSettingChangedFn fnOnValueChange = nullptr, bool isSlider = false, void* switchesArray = nullptr) = 0;
    virtual int ValueOfSettingsItem(int settingId) = 0;
};