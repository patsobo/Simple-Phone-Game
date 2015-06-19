#pragma once
#include "Sprite.h"
#include "Countdown.h"
//#include "Enemy.h"

class Enemy;

class Player :
	public Sprite
{
public:
	Player(ID3D11ShaderResourceView *m_Texture, Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds);

	void Attack(Enemy* enemy);	// Change to Enemy class when you create it
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
	Countdown* countdown;
};

