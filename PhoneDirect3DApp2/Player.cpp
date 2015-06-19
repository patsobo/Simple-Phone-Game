#include "pch.h"
#include "Player.h"
#include "Enemy.h"

Player::Player(ID3D11ShaderResourceView *m_Texture, Spritesheet* spritesheet, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds)
	: Sprite(spritesheet, m_Texture, position, movementBounds, 0, 10, 2)
{
	countdown = new Countdown(700, XMFLOAT2(0, 0));	// position doesn't matter
	defending = false;
	totalHealth = 80;
	health = totalHealth;
	damage = 10;
	dead = false;
}

void Player::Update(float timeTotal, float timeDelta)
{
	if (getAnimation() == 1)
	{
		position.x = 400;	// replace with approximate enemy position
		countdown->Update(timeTotal, timeDelta);
		if (countdown->isFinished())
		{
			position = initialPosition;
			setAnimation(0);
			countdown->resetTime();
		}
	}
	// If statement allows for pause on last frame if attacking (looks better than looping)
	if (!(animationPlayedOnce && getAnimation() == 1))
		Sprite::Update(timeTotal, timeDelta);
}

void Player::Attack(Enemy* enemy)
{
	if (getAnimation() == 1)
		countdown->resetTime();
	setAnimation(1);
	countdown->start();
	if(!enemy->isDefending())
	{
		enemy->decreaseHealth(damage);
	}
}

void Player::decreaseHealth(int delta)
{
	health -= delta;
	if (health <= 0)
		dead = true;
}

void Player::reset()
{
	health = totalHealth;
	dead = false;
}

bool Player::isDefending() { return defending; }
bool Player::isDead() { return dead; }
