#pragma once
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "SpriteText.h"

using namespace DirectX;

class Countdown
{
public:
	Countdown(float startTime, XMFLOAT2 position);

	void setTime(float newTime);	// set new time and 
	void resetTime();
	bool isFinished();
	void start();

	void Update(float timeTotal, float timeDelta);
	void Draw(SpriteBatch* spriteBatch, SpriteFont* spriteFont);
private:
	float time;	// In milliseconds
	float startTime;	// In milliseconds
	bool finished;	// True if timer has hit 0
	bool running; // True if timer is running
	XMFLOAT2 position;
};

