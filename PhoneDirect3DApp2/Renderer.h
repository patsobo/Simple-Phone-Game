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

const XMFLOAT2 PADDLE_DIM = XMFLOAT2(178, 401);
const XMFLOAT2 BALL_DIM = XMFLOAT2(500, 500);

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
	bool isGameRunning();
	void addToScore(int val);
	void movePaddles();
	void resetPaddles();

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

private:
	bool m_loadingComplete;
	bool gameStarted;
	int highScore;
	int score;
	void displayScores();
	void displayNum(int myScore, XMFLOAT2 position);
	int getNumDigits(int num);
	wchar_t const* numToWchar_t(int num);

	uint32 m_indexCount;

	unique_ptr<SpriteBatch> m_spriteBatch;

	//Accelerometer^ m_accelerometer;
	//AccelerometerReading^ m_accReading;

	float scale;

	// insert items here
	ID3D11ShaderResourceView* ballTexture;
	ID3D11ShaderResourceView* paddleTexture;
	Sprite* ball;
	Sprite* paddle1;
	Sprite* paddle2;
	Windows::Foundation::Rect paddleBounds;

	unique_ptr<SpriteFont> m_spriteFont;
};