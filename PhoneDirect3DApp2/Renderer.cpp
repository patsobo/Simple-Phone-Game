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
	gameStarted = false;
	scale = DisplayProperties::LogicalDpi / 96.0f;
	score = 0;
	highScore = 0; // To be replaced with memory call
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

	// Create the ball
	size = BALL_DIM;
	scale = .05f;
	position = XMFLOAT2(m_windowBounds.Width  / 2 - size.x * scale / 2, m_windowBounds.Height / 2 - size.y * scale / 2);
	speed = 200;
	//position = XMFLOAT2(100, 100);
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/ball.dds", nullptr, &ballTexture, MAXSIZE_T);
	ball = new Sprite(ballTexture, size, position, &m_windowBounds, scale, speed);
	
	// Create the paddles
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/paddle.dds", nullptr, &paddleTexture, MAXSIZE_T);
	size = PADDLE_DIM;
	scale = .4f;
	speed = 100;
	paddleBounds = m_windowBounds;
	paddleBounds.X = m_windowBounds.Width / 15;
	paddleBounds.Width = m_windowBounds.Width - paddleBounds.X*2;
	position = XMFLOAT2(paddleBounds.X, m_windowBounds.Height / 2 - size.y * scale / 2);
	paddle1 = new Sprite(paddleTexture, size, position, &paddleBounds, scale, speed);
	position.x = paddleBounds.X + paddleBounds.Width - size.x * scale;
	paddle2 = new Sprite(paddleTexture, size, position, &paddleBounds, scale, speed);
}

void Renderer::Update(float timeTotal, float timeDelta)
{
	if (gameStarted)
	{
		ball->Update(timeTotal, timeDelta);
		paddle1->Update(timeTotal, timeDelta);
		paddle2->Update(timeTotal, timeDelta);

		// Reverse direction if ball hits a paddle
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
				ball->setVelocity(XMFLOAT2(-1, 0));
			}
			ball->adjustPosition();	// Correction for minor collision
			addToScore(1);
		}
	}

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

	// Insert objects here
	if (gameStarted)
	{
		ball->Draw(m_spriteBatch.get());
		paddle1->Draw(m_spriteBatch.get());
		paddle2->Draw(m_spriteBatch.get());
		displayScores();
	}

	else if (!gameStarted)
	{
		float* stringlength = m_spriteFont->MeasureString(L"Tap to start!").n128_f32;

		m_spriteFont->DrawString(m_spriteBatch.get(), L"Tap to start!",
			XMFLOAT2(m_windowBounds.Width / 2.0f, m_windowBounds.Height / (5.0f / 2.0f)),
			Colors::Black, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
	}

	m_spriteBatch->End();
}

void Renderer::randomizeEnemies()
{
	//float distX = 0.0f, distY = 0.0f;
	//bool positionsOverlap = false;

	//for (int i = 0; i < 7; i++)
	//{
	//	float x = 50.0f + (float)rand() / ((float)RAND_MAX / (m_windowBounds.Width * scale - 50.0f));
	//	float y = 50.0f + (float)rand() / ((float)RAND_MAX / (m_windowBounds.Height * scale - 50.0f));

	//	//Check the new enemy won't overlap an old one
	//	for (iterator_enemies = enemies.begin(); iterator_enemies != enemies.end(); iterator_enemies++)
	//	{
	//		distX = x - (*iterator_enemies)->posX;
	//		distY = y - (*iterator_enemies)->posY;

	//		//Use ball diameter as helper value since the enemies' diameter is going to be the same
	//		if (((distX * distX) + (distY * distY)) <= (m_player->diameter * 2) * (m_player->diameter * 2))
	//		{
	//			positionsOverlap = true;
	//		}
	//	}

	//	if (!(x < goal->posX + goal->diameter && x > goal->posX - goal->diameter &&
	//		y < goal->posY + goal->diameter && y > goal->posY - goal->diameter) &&
	//		!(x < m_player->posX + m_player->diameter && x > m_player->posX - m_player->diameter &&
	//		y < m_player->posY + m_player->diameter && y > m_player->posY - m_player->diameter) &&
	//		!positionsOverlap)
	//	{
	//		enemies.push_back(new Enemy(x, y, &m_windowBounds));
	//	}
	//	else
	//	{
	//		i--;
	//		positionsOverlap = false;
	//	}
	//}
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

	float* digitLength = m_spriteFont->MeasureString(L"0").n128_f32;
	XMFLOAT2 position = XMFLOAT2(m_windowBounds.Width - m_windowBounds.Width / 20.0f, m_windowBounds.Height / 20.0f);
	displayNum(score, position);
	position.x = 0;
	for (int i = 0; i < getNumDigits(highScore); i++)
		position.x += *digitLength;
	displayNum(highScore, position);
}

void Renderer::displayNum(int myScore, XMFLOAT2 position)
{
	int numDigits = getNumDigits(myScore);
	wchar_t const* num;
	float* stringlength;
	float tempScore;
	for (int i = 0; i < numDigits; i++)
	{
		tempScore = myScore % 10;
		num = numToWchar_t(tempScore);
		stringlength = m_spriteFont->MeasureString(num).n128_f32;

		m_spriteFont->DrawString(m_spriteBatch.get(), num, position,
			Colors::Black, 0.0f, XMFLOAT2(*stringlength / 2.0f, 0.0f), 1.0f, DirectX::SpriteEffects_None, 0.0f);
		position.x -= *stringlength;

		myScore /= 10;
	}

}

int Renderer::getNumDigits(int num)
{
	if (num == 0) return 1;
	int counter = 0;
	if (num < 0) counter = 1; // remove this line if '-' counts as a digit
	while (num)
	{
		num /= 10;
		counter++;
	}
	return counter;
}

wchar_t const* Renderer::numToWchar_t(int num)
{
	switch (num)
	{
	case 0:
		return L"0";
		break;
	case 1:
		return L"1";
		break;
	case 2:
		return L"2";
		break;
	case 3:
		return L"3";
		break;
	case 4:
		return L"4";
		break;
	case 5:
		return L"5";
		break;
	case 6:
		return L"6";
		break;
	case 7:
		return L"7";
		break;
	case 8:
		return L"8";
		break;
	case 9:
		return L"9";
		break;
	default:
		return L"X";
		break;
	}
}

void Renderer::addToScore(int val)
{
	score += val;
}

void Renderer::resetGame()
{
	if (score > highScore)
		highScore = score;
	score = 0;
	ball->reset();
	paddle1->reset();
	paddle2->reset();
	
	gameStarted = false;
}

void Renderer::setGameRunning(bool running)
{
	gameStarted = running;
	ball->setVelocity(XMFLOAT2(1, 0));
}

bool Renderer::isGameRunning()
{
	return gameStarted;
}