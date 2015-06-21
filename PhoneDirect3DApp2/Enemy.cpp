#include "pch.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(ID3D11ShaderResourceView *m_Texture, Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds)
	: Sprite(spritesheet, m_Texture, position, movementBounds, 50, 6, .2)
{
	int defenseTime = rand() % 1000;
	int attackTime = rand() % 1000;
	defCount = new Countdown(defenseTime, XMFLOAT2(0, 0));	// position doesn't matter
	attCount = new Countdown(attackTime, XMFLOAT2(0, 0));
	dead = false;
	defending = false;
	attCount->start();
	XMFLOAT2 healthbarPosition = position;
	healthbarPosition.y -= 35;
	healthbar = new Healthbar(healthbarPosition, 80);
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
		spriteEffects = SpriteEffects_None;
		defCount->resetTime();
		attCount->start();
	}
	Sprite::Update(timeTotal, timeDelta);
}

void Enemy::Draw(SpriteBatch* spriteBatch)
{
	healthbar->Draw(spriteBatch);
	Sprite::Draw(spriteBatch);
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
	healthbar->decreaseHealth(delta);
	dead = healthbar->hasNoHealth();
}

void Enemy::reset()
{
	healthbar->reset();
	int defenseTime = rand() % 1000;
	int attackTime = rand() % 1000;
	defCount->setTime(defenseTime);
	attCount->setTime(attackTime);
	attCount->start();
	dead = false;
}

void Enemy::LoadHealthbar(ID3D11Device* d3dDevice)
{
	healthbar->LoadTexture(d3dDevice);
}

bool Enemy::isDefending() { return defending; }
bool Enemy::isDead() { return dead; }
