#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include <sautils.h>

MYMODCFG(net.rusjj.gtasa.utils, GTA:SA Utils, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGTASA = 0;

extern "C" void OnModPreLoad() // A place for interfaces registering
{
    logger->SetTag("GTASA Utils");
    pGTASA = aml->GetLib("libGTASA.so");

    ((SAUtils*)sautils)->m_bHasFLA = aml->GetLib("libplugin_fastman92limitAdjuster_ANDROID_ARM32.so") != 0;
    
    RegisterInterface("SAUtils", sautils);
}