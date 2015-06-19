#include "pch.h"
#include "Countdown.h"


Countdown::Countdown(float startTime, XMFLOAT2 position)
{
	this->startTime = startTime;
	this->position = position;
	time = startTime;
	finished = false;
	running = false;
}

void Countdown::start()
{
	running = true;
	finished = false;
}

void Countdown::setTime(float newTime)
{
	startTime = newTime;
}

void Countdown::resetTime()
{
	time = startTime;
	running = false;
	finished = false;
}

void Countdown::Update(float timeTotal, float timeDelta)
{
	if (running)
	{
		time -= timeDelta*1000;
	}

	if (time <= 0)
	{
		running = false;
		finished = true;
	}
}

void Countdown::Draw(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	int seconds = time / 1000.0f;
	int centiseconds = ((int)time % 1000)/10.0f;

	XMFLOAT2 tempPosition = position;

	// Position is weird because position of individual letter is top left corner, but numbers
	// start drawing from the least significant digit!
	SpriteText::displayNum(spriteBatch, spriteFont, seconds, tempPosition, 2);
	float* digitLength = spriteFont->MeasureString(L"0").n128_f32;
	float* stringLength = spriteFont->MeasureString(L"\"").n128_f32;
	tempPosition.x += *digitLength / 2 + (*stringLength)/2;
	spriteFont->DrawString(spriteBatch, L"\"", tempPosition,
		Colors::Black, 0.0f, XMFLOAT2(*stringLength / 2.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
	tempPosition.x += (*stringLength)/2;
	// 2 because that is the zero fill
	tempPosition.x += (*digitLength)*(SpriteText::getNumDigits(centiseconds) > 2 ? SpriteText::getNumDigits(centiseconds) : 2);
	SpriteText::displayNum(spriteBatch, spriteFont, centiseconds, tempPosition, 2);
}

bool Countdown::isFinished()
{
	return finished;
}
bool Countdown::isRunning() { return running; }
