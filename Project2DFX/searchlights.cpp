#include <searchlights.h>
#include <cmath>
#include <stdint.h>

extern float* flWeatherFoggyness;
extern int* nActiveInterior;
extern void* TheCamera;
extern uintptr_t pGTASA;

extern char(*GetIsTimeInRange)(char hourA, char hourB);


std::vector<CLamppostInfo>					Lampposts;
std::map<unsigned int, CLamppostInfo>		FileContent;
std::vector<CLamppostInfo>*					m_pLampposts = &Lampposts;
std::map<unsigned int, CLamppostInfo>*		pFileContent = &FileContent;
extern float 								fSearchlightEffectVisibilityFactor;


void CSearchLights::RenderSearchLightsSA()
{
	/* It needs a full rework */
    /*if(*flWeatherFoggyness)
    {
        if (GetIsTimeInRange(20, 7) && *nActiveInterior == 0)
        {
            static auto SetRenderStatesForSpotLights = (void(*)()) (pGTASA + 0x194980);
			SetRenderStatesForSpotLights();

			// 0x952126 word_952126
			// 0x95275C word_95275C
            for (auto it = m_pLampposts->cbegin(); it != m_pLampposts->cend(); ++it)
			{
				if (it->nDrawSearchlight)
				{
					// CCamera* + 2264 = m_pRwCamera
					CVector*	pCamPos = (&TheCamera + 2264);
					float		fDistSqr = (pCamPos->x - it->vecPos.x)*(pCamPos->x - it->vecPos.x) + (pCamPos->y - it->vecPos.y)*(pCamPos->y - it->vecPos.y) + (pCamPos->z - it->vecPos.z)*(pCamPos->z - it->vecPos.z);

					if ((fDistSqr > 50.0f*50.0f) && (fDistSqr < 300.0f*300.0f))
					{
						float fVisibility = fSearchlightEffectVisibilityFactor * ((0.0233333f)*sqrt(fDistSqr) - 1.16667f);

						RwV3D EndPoint = *(RwV3D*)&it->vecPos;
						EndPoint.z = CLODLightManager::SA::FindGroundZFor3DCoord(it->vecPos.x, it->vecPos.y, it->vecPos.z, 0, 0);

						if (!(it->colour.r == 255 && it->colour.g == 255 && it->colour.b == 255) && !(it->colour.r == 254 && it->colour.g == 117 && it->colour.b == 134))
						{
							//yellow
							DrawCustomSpotLightSA(*(RwV3D*)&it->vecPos, EndPoint, min((8.0f * (it->vecPos.z - EndPoint.z)), 90.0f), it->fCustomSizeMult / 6.0f, 5.0f, 8, fVisibility);
						}
						else if (!(it->colour.r == 254 && it->colour.g == 117 && it->colour.b == 134))
						{
							//white
							DrawCustomSpotLightSA(*(RwV3D*)&it->vecPos, EndPoint, min((8.0f * (it->vecPos.z - EndPoint.z)), 90.0f), it->fCustomSizeMult / 6.0f, 255.0f, 8, fVisibility);
						}
						else
						{
							//pink
							DrawCustomSpotLightSA(*(RwV3D*)&it->vecPos, EndPoint, min((8.0f * (it->vecPos.z - EndPoint.z)), 90.0f), it->fCustomSizeMult / 6.0f, 200.0f, 18, fVisibility);
						}
					}
				}
			}


			static auto ResetRenderStatesForSpotLights = (void(*)()) (pGTASA + 0x196C1C);
			ResetRenderStatesForSpotLights();
        }
    }*/
}

void CSearchLights::DrawCustomSpotLightSA(RwV3D StartPoint, RwV3D EndPoint, float TargetRadius, float baseRadius, float slColorFactor1, char slColorFactor2, float slAlpha)
{
	
}

void CSearchLights::RegisterLamppost(CEntity* pObj)
{
    /*unsigned short      nModelID = pObj->GetModelIndex();
    CMatrix             dummyMatrix;
    CSimpleTransform&   objTransform = pObj->GetTransform();

    if (objTransform.m_translate.x == 0.0f && objTransform.m_translate.y == 0.0f)
        return;

    dummyMatrix.SetTranslateOnly(objTransform.m_translate.x, objTransform.m_translate.y, objTransform.m_translate.z);
    dummyMatrix.SetRotateZOnly(objTransform.m_heading);

    auto    itEnd = pFileContent->upper_bound(PackKey(nModelID, 0xFFFF));
    for (auto it = pFileContent->lower_bound(PackKey(nModelID, 0)); it != itEnd; it++)
        m_pLampposts->push_back(CLamppostInfo(dummyMatrix * it->second.vecPos, it->second.colour, it->second.fCustomSizeMult, it->second.nCoronaShowMode, it->second.nNoDistance, it->second.nDrawSearchlight, pObj->GetTransform().m_heading));
*/}