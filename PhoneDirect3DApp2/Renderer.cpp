#include "pch.h"
#include "Renderer.h"
#include <time.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;

Renderer::Renderer() :
m_loadingComplete(false),
m_indexCount(0)
{
	gameState = GameState::Initial;
	scale = DisplayProperties::LogicalDpi / 96.0f;
	score = 0;
	highScore = 0; // To be replaced with memory call
	srand((unsigned)time(0));
}

Renderer::Renderer(int highScore) :
m_loadingComplete(false),
m_indexCount(0)
{
	gameState = GameState::Initial;
	scale = DisplayProperties::LogicalDpi / 96.0f;
	score = 0;
	this->highScore = highScore; // To be replaced with memory call
	srand((unsigned)time(0));
}

void Renderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();

	m_spriteBatch = unique_ptr<SpriteBatch>(new
		DirectX::SpriteBatch(m_d3dContext.Get()));

	m_spriteFont = unique_ptr<SpriteFont>(new SpriteFont(m_d3dDevice.Get(), L"Assets/deluxe.spritefont"));
}

void Renderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	m_windowBounds.Height *= scale;
	m_windowBounds.Width *= scale;

	// Values to be used for initializing each sprite (made for readability purposes)
	XMFLOAT2 size;
	XMFLOAT2 position;
	float scale;
	float speed;

	// Create the pause button
	size = PAUSE_DIM;
	scale = 2.0f;
	position = XMFLOAT2(20, 20);
	speed = 0;
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/pauseButton.dds", nullptr, &pauseButtonTexture, MAXSIZE_T);
	pauseButton = new Sprite(pauseButtonTexture, size, position, &m_windowBounds , scale, speed);

	// Create the background
	size = BGD_DIM;
	float scaleX = m_windowBounds.Width / size.x;
	float scaleY = m_windowBounds.Height / size.y;
	scale = scaleX > scaleY ? scaleX : scaleY;
	position = XMFLOAT2(0, 0);
	speed = 0;
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/background.dds", nullptr, &backgroundTexture, MAXSIZE_T);
	background = new Sprite(backgroundTexture, size, position, &m_windowBounds, scale, speed);	// Technically, m_windowBounds is too small, but, eh...

	// Create the ball
	size = BALL_DIM;
	scale = .05f;
	position = XMFLOAT2(m_windowBounds.Width / 2 - size.x * scale / 2, m_windowBounds.Height * 3 / 5 - size.y * scale / 2);
	speed = 250;
	//position = XMFLOAT2(100, 100);
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/ball.dds", nullptr, &ballTexture, MAXSIZE_T);
	ball = new Sprite(ballTexture, size, position, &m_windowBounds, scale, speed);
	
	// Create the paddles
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/paddle.dds", nullptr, &paddleTexture, MAXSIZE_T);
	size = PADDLE_DIM;
	scale = .4f;
	speed = 150;
	paddleBounds = m_windowBounds;
	paddleBounds.X = m_windowBounds.Width / 15;
	paddleBounds.Width = m_windowBounds.Width - paddleBounds.X*2;
	position = XMFLOAT2(paddleBounds.X, m_windowBounds.Height * 3 / 5 - size.y * scale / 2);
	paddle1 = new Sprite(paddleTexture, size, position, &paddleBounds, scale, speed);
	position.x = paddleBounds.X + paddleBounds.Width - size.x * scale;
	paddle2 = new Sprite(paddleTexture, size, position, &paddleBounds, scale, speed);

	// Create the player
	vector<XMFLOAT2> sizeOfPlayer(4);
	vector<int> playerFrames(4);
	for (int i = 0; i < 4; i++)
	{
		sizeOfPlayer[i] = XMFLOAT2(128.0f / 4, 192.0f / 4);
		playerFrames[i] = 4;
	}
	XMFLOAT2 positionOfPlayer((m_windowBounds.Width / 4) - (sizeOfPlayer[0].x * 2/ 2), (m_windowBounds.Height * 2 / 5) - (sizeOfPlayer[0].y * 2 / 2));

	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/Link.dds", nullptr, &playerTexture, MAXSIZE_T);
	playerSheet = new Spritesheet(sizeOfPlayer, 4, playerFrames, 128);
	player = new Player(playerTexture, playerSheet, positionOfPlayer, &m_windowBounds);
	player->LoadHealthbar(m_d3dDevice.Get());
	
	// Create the enemy
	size = XMFLOAT2(472, 467);
	XMFLOAT2 positionOfEnemy((m_windowBounds.Width * 3 / 4) - (size.x * .2 / 2), (m_windowBounds.Height * 2 / 5) - (size.y * .2 / 2));
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/eds.dds", nullptr, &enemyTexture, MAXSIZE_T);
	enemySheet = new Spritesheet(size);
	enemy = new Enemy(enemyTexture, enemySheet, positionOfEnemy, &m_windowBounds);
	enemy->LoadHealthbar(m_d3dDevice.Get());

	countdown = new Countdown(START_TIME_MILLI, XMFLOAT2(m_windowBounds.Width / 2, m_windowBounds.Height / 5));

	// Audio
	// Create DirectXTK for Audio objects 
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG 
	eflags = eflags | AudioEngine_Debug;
#endif 
	m_audEngine.reset(new AudioEngine(eflags));

	m_audioEvent = 0;
	m_audioTimerAcc = 10.f;
	m_retryDefault = false;

	//m_waveBank.reset(new WaveBank(m_audEngine.get(), L"assets\\adpcmdroid.xwb"));
	m_musicFile.reset(new SoundEffect(m_audEngine.get(), L"Assets/Windfish Music.wav"));
	m_music = m_musicFile->CreateInstance();
	//m_effect2 = m_waveBank->CreateInstance(10);

	m_music->Play(true);
	//m_effect2->Play();
}

void Renderer::Update(float timeTotal, float timeDelta)
{
	HandleAudio(timeTotal, timeDelta);
	if (gameState == GameState::InGameActive)
		HandleGameplay(timeTotal, timeDelta);
}

void Renderer::HandleAudio(float timeTotal, float timeDelta)
{
	m_audioTimerAcc -= timeDelta;
	if (m_audioTimerAcc < 0)
	{
		if (m_retryDefault)
		{
			m_retryDefault = false;
			if (m_audEngine->Reset())
			{
				// Restart looping audio 
				m_music->Play(true);
			}
		}
		else
		{
			// Unused atm
			m_audioTimerAcc = 4.f;

			//m_waveBank->Play(m_audioEvent++);

			if (m_audioEvent >= 11)
				m_audioEvent = 0;
		}
	}

	if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
	{
		// Setup a retry in 1 second (can happen when transitioning to and from a BlueTooth audio connection) 
		m_audioTimerAcc = 1.f;
		m_retryDefault = true;
	}
}

void Renderer::HandleGameplay(float timeTotal, float timeDelta)
{
	pauseButton->Update(timeTotal, timeDelta);
	background->Update(timeTotal, timeDelta);
	countdown->Update(timeTotal, timeDelta);
	ball->Update(timeTotal, timeDelta);
	paddle1->Update(timeTotal, timeDelta);
	paddle2->Update(timeTotal, timeDelta);
	player->Update(timeTotal, timeDelta);
	enemy->Update(timeTotal, timeDelta);

	if (enemy->isDead())
	{
		addToScore(1);
		enemy->reset();
	}

	// Reverse direction if ball hits a paddle
	// This entire block is essentially the collision detector
	if (ball->CollidesWith(paddle1) || ball->CollidesWith(paddle2))
	{
		if (ball->CollidesWith(paddle1) && ball->CollidesWith(paddle2))
		{
			resetGame();
			return;
		}
		if (ball->CollidesWith(paddle1))
		{
			while (ball->CollidesWith(paddle1))
			{
				ball->adjustPosition();
				if (ball->CollidesWith(paddle1) && ball->CollidesWith(paddle2))
				{
					resetGame();
					return;
				}
			}
			player->Attack(enemy);
			ball->setVelocity(XMFLOAT2(1, 0));
		}
		else
		{
			while (ball->CollidesWith(paddle2))
			{
				ball->adjustPosition();
				if (ball->CollidesWith(paddle1) && ball->CollidesWith(paddle2))
				{
					resetGame();
					return;
				}
			}
			player->Attack(enemy);
			ball->setVelocity(XMFLOAT2(-1, 0));
		}
		ball->adjustPosition();	// Correction for minor collision
		//addToScore(1);
	}

	// Check if countdown is over
	if (countdown->isFinished())
		resetGame();

	// insert reset game logic
}

void Renderer::Render()
{
	const float bg_color[] = { 245.0f / 255.0f, 241.0 / 255.0f, 196.0f / 255.0f, 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		bg_color
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	m_spriteBatch->Begin();

	background->Draw(m_spriteBatch.get());
	if (gameState == GameState::Initial)
	{
		float* stringlength = m_spriteFont->MeasureString(L"Tap to start!").m128_f32;

		m_spriteFont->DrawString(m_spriteBatch.get(), L"Tap to start!",
			XMFLOAT2(m_windowBounds.Width / 2.0f, m_windowBounds.Height / (5.0f / 2.0f)),
			Colors::Black, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
	}
	else if (gameState == GameState::InGameActive || gameState == GameState::InGamePaused)
	{
		ball->Draw(m_spriteBatch.get());
		paddle1->Draw(m_spriteBatch.get());
		paddle2->Draw(m_spriteBatch.get());
		player->Draw(m_spriteBatch.get());
		if (!enemy->isDead())
			enemy->Draw(m_spriteBatch.get());
		countdown->Draw(m_spriteBatch.get(), m_spriteFont.get());
		pauseButton->Draw(m_spriteBatch.get());
		displayScores();
	}

	m_spriteBatch->End();
}

void Renderer::randomizeEnemies()
{
	// TODO: Initialize behavior/placement of enemies if you ever decide to have enemies.
}

void Renderer::movePaddles()
{
	paddle1->setVelocity(XMFLOAT2(1, 0));
	paddle2->setVelocity(XMFLOAT2(-1, 0));
}

void Renderer::resetPaddles()
{
	paddle1->setVelocity(XMFLOAT2(-1, 0));
	paddle2->setVelocity(XMFLOAT2(1, 0));
}

void Renderer::displayScores()
{
	if (score > highScore)
		highScore = score;

	float* digitLength = m_spriteFont->MeasureString(L"0").m128_f32;
	XMFLOAT2 position = XMFLOAT2(m_windowBounds.Width - m_windowBounds.Width / 20.0f, m_windowBounds.Height / 20.0f + pauseButton->getHeight() + 20);	// replace 10 with variable
	float *stringlength = m_spriteFont->MeasureString(L"Score").m128_f32;
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Score",
		position, Colors::Black, 0.0f, XMFLOAT2(*stringlength, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);

	position.y += 32;
	SpriteText::displayNum(m_spriteBatch.get(), m_spriteFont.get(), score, position, 0);
	position.x = m_windowBounds.Width / 20;
	position.y -= 32;
	m_spriteFont->DrawString(m_spriteBatch.get(), L"High Score",
		position, Colors::Black, 0.0f, XMFLOAT2(0.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
	for (int i = 0; i < SpriteText::getNumDigits(highScore) - 1; i++)
		position.x += *digitLength;
	position.y += 32;
	SpriteText::displayNum(m_spriteBatch.get(), m_spriteFont.get(), highScore, position, 0);
}

void Renderer::addToScore(int val)
{
	score += val;
}

void Renderer::resetGame()
{
	gameState = GameState::Initial;
	if (score > highScore)
		highScore = score;
	score = 0;
	ball->reset();
	paddle1->reset();
	paddle2->reset();
	player->reset();
	enemy->reset();
	countdown->resetTime();
}

void Renderer::HandlePressInput(Windows::UI::Input::PointerPoint^ currentPoint)
{
	XMFLOAT2 vectorPoint = XMFLOAT2(currentPoint->RawPosition.X, currentPoint->RawPosition.Y);

	switch (gameState)
	{
	case GameState::InGameActive:
		if (!onButton(pauseButton, vectorPoint))
			movePaddles();
		break;
	case GameState::InGamePaused:
		break;
	case GameState::Initial:
		break;
	default:
		break;
	}
}

void Renderer::HandleReleaseInput(Windows::UI::Input::PointerPoint^ currentPoint)
{
	XMFLOAT2 vectorPoint = XMFLOAT2(currentPoint->RawPosition.X, currentPoint->RawPosition.Y);

	switch (gameState)
	{
	case GameState::InGameActive:
		if (onButton(pauseButton, vectorPoint))
			gameState = GameState::InGamePaused;
		else
		{
			resetPaddles();
		}
		break;
	case GameState::InGamePaused:
		if (onButton(pauseButton, vectorPoint))
		{
			resetPaddles();
			gameState = GameState::InGameActive;
		}
		break;
	case GameState::Initial:
		setGameRunning(true);
		gameState = GameState::InGameActive;
		break;
	default:
		break;
	}
}

bool Renderer::onButton(Sprite* button, XMFLOAT2 pointer)
{
	Point point = Point(pointer.x, pointer.y);
	if (button->getBoundingBox()->Contains(point))
		return true;
	return false;
}

void Renderer::setGameRunning(bool running)
{
	ball->setVelocity(XMFLOAT2(1, 0));
	countdown->start();
}

int Renderer::getHighScore() { return highScore; }
void Renderer::setHighScore(int newScore) { highScore = newScore; }