#pragma once
#include "Sprite.h"
#include "Countdown.h"
//#include "Player.h"

class Player;

class Enemy :
	public Sprite
{
public:
	Enemy(ID3D11ShaderResourceView *m_Texture, Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds);

	void Attack(Player* player);
	void decreaseHealth(int delta);
	bool isDefending();
	bool isDead();
	void reset();

	void Update(float timeTotal, float timeDelta);

private:
	int health, totalHealth;
	int damage;
	bool dead;
	bool defending;
	Countdown* defCount;	// Tracks how long defense should last
	Countdown* attCount;	// Tracks how long attack should last
};

