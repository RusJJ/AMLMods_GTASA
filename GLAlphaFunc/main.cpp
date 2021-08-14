#include <mod/amlmod.h>
#include <mod/logger.h>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

MYMOD(net.rusjj.glalphafunc, GLAlphaFunc CrashFix, 1.0, RusJJ)
NEEDGAME(com.rockstargames.gtasa)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0)
END_DEPLIST()

uintptr_t pGTASA = 0;
extern "C" void OnModLoad()
{
    pGTASA = aml->GetLib("libGTASA.so");
    aml->Unprot(pGTASA + 0x6BCBF8, sizeof(void*));
    *(void**)(pGTASA + 0x6BCBF8) = (void*)glAlphaFuncQCOM;
}