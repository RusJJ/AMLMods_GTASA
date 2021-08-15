#ifndef _SEARCHLIGHTS
#define _SEARCHLIGHTS

#include <vector>
#include <map>

#include <gtasa_engine/cvector.h>
#include <gtasa_engine/crgba.h>
#include <gtasa_engine/RwV3D.h>

class CEntity;
class CLamppostInfo;

extern std::vector<CLamppostInfo>* m_pLampposts;
extern std::map<unsigned int, CLamppostInfo>* pFileContent;

class CLamppostInfo
{
public:
	CVector			vecPos;
	CRGBA			colour;
	float			fCustomSizeMult;
	int 			nNoDistance;
	int             nDrawSearchlight;
	float			fHeading;
	int				nCoronaShowMode;

	CLamppostInfo(const CVector& pos, const CRGBA& col, float fCustomMult, int CoronaShowMode, int nNoDistance, int nDrawSearchlight, float heading)
		: vecPos(pos), colour(col), fCustomSizeMult(fCustomMult), nCoronaShowMode(CoronaShowMode), nNoDistance(nNoDistance), nDrawSearchlight(nDrawSearchlight), fHeading(heading)
	{}
};

class CSearchLights
{
public:
	static void DrawCustomSpotLightSA(RwV3D StartPoint, RwV3D EndPoint, float TargetRadius, float baseRadius, float slColorFactor1, char slColorFactor2, float slAlpha = 1.0f);
	static void	RenderSearchLightsSA();
	static void RegisterLamppost(CEntity* pObj);
};

#endif // _SEARCHLIGHTS