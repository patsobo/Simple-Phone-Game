#pragma once

#include <wrl\client.h>
#include <memory>

#include "Direct3DBase.h"
#include <directxmath.h>
#include <list>

#include "Audio.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "Countdown.h"
#include "GameState.h"
#include "Player.h"
#include "Enemy.h"

using namespace DirectX;
using namespace Windows::Devices::Sensors;
using namespace std;

const XMFLOAT2 PAUSE_DIM = XMFLOAT2(32, 38);
const XMFLOAT2 BGD_DIM = XMFLOAT2(768, 1152);
const XMFLOAT2 PADDLE_DIM = XMFLOAT2(178, 401);
const XMFLOAT2 BALL_DIM = XMFLOAT2(500, 500);
const int START_TIME_MILLI = 15000;

ref class Renderer sealed : public Direct3DBase
{
public:
	Renderer();
	Renderer(int highScore);

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	void randomizeEnemies();
	void resetGame();
	void setGameRunning(bool gameStarted);
	void addToScore(int val);
	void movePaddles();
	void resetPaddles();
	int getHighScore();
	void setHighScore(int newScore);

	void HandlePressInput(Windows::UI::Input::PointerPoint^ currentPoint);	// called when pointer is down, alters state
	void HandleReleaseInput(Windows::UI::Input::PointerPoint^ currentPoint);	// called when pointer is released, alters state

	// Method for updating time-dependent objects.
	// Note: time floats are in seconds
	void Update(float timeTotal, float timeDelta);

private:
	GameState gameState;

	void HandleAudio(float timeTotal, float timeDelta);
	void HandleGameplay(float timeTotal, float timeDelta);
	bool onButton(Sprite* sprite, XMFLOAT2 point);

	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	//std::unique_ptr<DirectX::WaveBank> m_waveBank;
	std::unique_ptr<DirectX::SoundEffect> m_musicFile;
	std::unique_ptr<DirectX::SoundEffectInstance> m_music;
	//std::unique_ptr<DirectX::SoundEffectInstance> m_effect2;

	uint32_t m_audioEvent;
	float m_audioTimerAcc;
	bool m_retryDefault;

	bool m_loadingComplete;
	int highScore;
	int score;
	void displayScores();

	uint32 m_indexCount;

	unique_ptr<SpriteBatch> m_spriteBatch;

	//Accelerometer^ m_accelerometer;
	//AccelerometerReading^ m_accReading;

	float scale;

	// insert items here
	ID3D11ShaderResourceView* backgroundTexture;
	ID3D11ShaderResourceView* ballTexture;
	ID3D11ShaderResourceView* paddleTexture;
	ID3D11ShaderResourceView* pauseButtonTexture;
	ID3D11ShaderResourceView* playerTexture;
	ID3D11ShaderResourceView* enemyTexture;
	Spritesheet* playerSheet;
	Spritesheet* enemySheet;
	Sprite* ball;
	Sprite* paddle1;
	Sprite* paddle2;
	Sprite* background;
	Sprite* pauseButton;
	Player* player;
	Enemy* enemy;
	Windows::Foundation::Rect paddleBounds;
	Countdown* countdown;

	unique_ptr<SpriteFont> m_spriteFont;
};