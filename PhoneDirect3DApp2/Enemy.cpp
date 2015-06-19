#include "pch.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(ID3D11ShaderResourceView *m_Texture, Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds)
	: Sprite(spritesheet, m_Texture, position, movementBounds, 50, 6, .2)
{
	int defenseTime = rand() % 2500;
	int attackTime = rand() % 2000;
	defCount = new Countdown(500, XMFLOAT2(0, 0));	// position doesn't matter
	attCount = new Countdown(500, XMFLOAT2(0, 0));
	dead = false;
	defending = false;
	attCount->start();
	totalHealth = 80;
	health = totalHealth;
	damage = 10;
}

void Enemy::Update(float timeTotal, float timeDelta)
{
	if (attCount->isRunning())
	{
		defending = false;
		attCount->Update(timeTotal, timeDelta);
	}
	if (attCount->isFinished())
	{
		defending = true;
		spriteEffects = SpriteEffects_FlipVertically;
		attCount->resetTime();
		defCount->start();
	}
	if (defCount->isRunning())
	{
		defending = true;
		defCount->Update(timeTotal, timeDelta);
	}
	if (defCount->isFinished())
	{
		defending = false;
		spriteEffects = SpriteEffects_FlipVertically;
		defCount->resetTime();
		attCount->start();
	}
	Sprite::Update(timeTotal, timeDelta);
}

void Enemy::Attack(Player* player)
{
	if(!player->isDefending())
	{
		player->decreaseHealth(damage);
	}
}

void Enemy::decreaseHealth(int delta)
{
	health -= delta;
	if (health <= 0)
		dead = true;
}

void Enemy::reset()
{
	health = totalHealth;
	dead = false;
}

bool Enemy::isDefending() { return defending; }
bool Enemy::isDead() { return dead; }
