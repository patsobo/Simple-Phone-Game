#pragma once

#include <wrl\client.h>
#include <memory>

#include "Direct3DBase.h"
#include <directxmath.h>
#include <list>

#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Sprite.h"

using namespace DirectX;
using namespace Windows::Devices::Sensors;
using namespace std;

ref class Renderer sealed : public Direct3DBase
{
public:
	Renderer();

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	void randomizeEnemies();
	void resetGame();
	void setGameRunning(bool gameStarted);
	bool getGameRunning();

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

private:
	bool m_loadingComplete;
	bool gameStarted;

	uint32 m_indexCount;

	unique_ptr<SpriteBatch> m_spriteBatch;

	//Accelerometer^ m_accelerometer;
	//AccelerometerReading^ m_accReading;

	float scale;

	// insert items here
	Sprite* ball;
	Sprite* paddle1;
	Sprite* paddle2;

	unique_ptr<SpriteFont> m_spriteFont;
};