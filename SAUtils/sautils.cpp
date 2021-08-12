#include <sautils.h>

bool SAUtils::IsFLALoaded()
{
    return m_bHasFLA;
}

static SAUtils sautilsLocal;
ISAUtils* sautils = &sautilsLocal;