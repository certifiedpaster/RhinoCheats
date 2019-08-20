#pragma once

class NoSpread_t
{
public:
	void ApplyNoSpread(usercmd_t * cmd, int seed);	

private:
	void FirstBulletFix();
	void GetWeaponSpread(float * Spread);

	float maxSpread, baseSpread, weaponSpread, spreadX, spreadY;
	
};

extern NoSpread_t Nospread;


