#include "stdafx.h"
#include "EndFrame.h"

/************************************************************************/
/* Init                                                                 */
/************************************************************************/

void Init()
{
	Entity.bones.clear();

	for (int i = 0; i < NUM_TAGS; i++)
	{		
		WORD bone = Engine.RegisterTag(Entity.tagNames[i].c_str());							
		Entity.bones.push_back(bone);			
	}	
}

/************************************************************************/
/* Render                                                               */
/************************************************************************/

void RenderAll()
{
	if (!cg || !cg->snap || cg->snap->snapFlags & SNAPFLAG_NOT_ACTIVE)
		return;
	if (!refdef)
		return;

	static DWORD lastFrameTickCount = 0;	

	if ((lastFrameTickCount + ENDFRAME_TICKCOUNT_MAX_DIFF) < GetTickCount())	
		Init();
		

	Engine.DamageFeedBack_Removal();	

	Entity.DrawEsp();	
	
	Drawing.DrawCrossHair(0);	

	Drawing.DrawHealth();	

	lastFrameTickCount = GetTickCount();
	
}

