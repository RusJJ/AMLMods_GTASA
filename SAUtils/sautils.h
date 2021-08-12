#include <isautils.h>

class SAUtils : public ISAUtils
{
public:
    bool IsFLALoaded();

public:
    bool m_bHasFLA;
};
extern ISAUtils* sautils;