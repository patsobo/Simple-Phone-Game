#pragma once
#include "SpriteFont.h"

using namespace DirectX;

ref class Countdown sealed
{
public:
	Countdown(float startTime);

	void setTime(float newTime);	// set new time and 
	void resetTime();
	bool isFinished();

	void Update(float timeTotal, float timeDelta);
	void Draw(Renderer renderer, SpriteFont spriteFont);
private:
	float time;	// In milliseconds
	float startTime;	// In milliseconds
	bool finished;	// True if timer has hit 0
	bool running; // True if timer is running
};

