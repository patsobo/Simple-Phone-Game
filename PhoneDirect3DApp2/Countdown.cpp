#include "pch.h"
#include "Countdown.h"


Countdown::Countdown(float startTime)
{
	this->startTime = startTime;
	time = startTime;
	finished = false;
	running = false;
}

void Countdown::setTime(float newTime)
{
	startTime = newTime;
}

void Countdown::resetTime()
{
	time = startTime;
	running = false;
}

void Countdown::Update(float timeTotal, float timeDelta)
{
	if (running)
	{

	}

	if (time == 0)
	{
		finished = true;
	}
}

void Countdown::Draw(SpriteFont spriteFont)
{

}

bool Countdown::isFinished()
{
	return finished;
}
