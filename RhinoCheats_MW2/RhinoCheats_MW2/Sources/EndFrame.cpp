#include "stdafx.h"
#include "EndFrame.h"

MTRand mtRandom;

/************************************************************************/
/* Init                                                                 */
/************************************************************************/

void Init()
{
	static bool bOnce = false;
	if (!bOnce)
	{
		HeadShotIcon = RegisterShader("killiconheadshot");
		DiedIcon = RegisterShader("cardicon_prpurchase_1");
		ScavengerIcon = RegisterShader("scavenger_pickup");
		FlashbangIcon = RegisterShader("hud_flashbangicon");

		Engine.ApplyDvars();		

		bOnce = true;
	}
	
	//Admins can modify player model (possible crash).
	for (int i = 0; i < NUM_TAGS; i++)
	{

		WORD bone = Engine.RegisterTag(Entity.tagNames[i].c_str());
		if (bone <= 0)
		{
			//Should work but it doesn't :(
			//disconnect();

			SafeMessageBox(0, "The players' models have been modified.\nDisconnecting to avoid possible crash.", ALASKA17, MB_ICONSTOP);

			Engine.SendToConsole("disconnect");
			return;
		}

		Entity.bones.push_back(bone);

	}

	Aim.SetTime4NameChange(0);

	Engine.FuckStunFlashShit(concGrenade);
	Engine.FuckStunFlashShit(flashGrenade);

	KillSpam.Init();
	KillSpree.Init();

	sounds.bGetValuesOnce = true;
}




/************************************************************************/
/* Render                                                               */
/************************************************************************/

void RenderAll()
{
	if (!cg)
		return;

	static DWORD lastFrameTickCount = 0;	

	if ((lastFrameTickCount + ENDFRAME_TICKCOUNT_MAX_DIFF) < GetTickCount())
		Init();

	Engine.DamageFeedBack_Removal();

	Aim.Run();	

	Entity.DrawEsp();

	Engine.Misc_Process();	
	
	Engine.DrawLaser();		
	
	Drawing.InitializeRainBow();
	Drawing.DrawCrossHair(Settings[draw_crosshair].Value.iValue);	
	if (Settings[watermark].Value.bValue)
	{
		Drawing.DrawGlowString(refdef->width / 2.0f, 40, Fonts->hudsmallfont, 1.7f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 1.0f, 1.0f, 1.0f), 0, true, ALASKA17);
		Drawing.DrawHealth();
	}
	Drawing.DrawClock();	

	Host::Process();

	Engine.AFK();
	
	com_maxfps->setFloat(0);

	lastFrameTickCount = GetTickCount();

}
