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

	// Values to be used for initializing each sprite (made for readability purposes)
	XMFLOAT2 size;
	XMFLOAT2 position;
	float scale;

	// Create the ball
	size = XMFLOAT2(500, 500);
	scale = .05f;
	position = XMFLOAT2(m_windowBounds.Width / 2 - size.x * scale / 2, m_windowBounds.Height / 2 - size.y * scale / 2);
	//position = XMFLOAT2(100, 100);
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/ball.dds", nullptr, &ballTexture, MAXSIZE_T);
	ball = new Sprite(ballTexture, size, position, &m_windowBounds, scale);
	
	// Create the paddles
	CreateDDSTextureFromFile(m_d3dDevice.Get(), L"Assets/paddle.dds", nullptr, &paddleTexture, MAXSIZE_T);
	size = XMFLOAT2(178, 401);
	scale = .4f;
	position = XMFLOAT2(m_windowBounds.Width / 8 - size.x * scale / 2, m_windowBounds.Height / 2 - size.y * scale / 2);
	paddle1 = new Sprite(paddleTexture, size, position, &m_windowBounds, scale);
	position.x = m_windowBounds.Width * 7 / 8 - size.x * scale / 2;
	paddle2 = new Sprite(paddleTexture, size, position, &m_windowBounds, scale);
}

void Renderer::Update(float timeTotal, float timeDelta)
{
	if (gameStarted)
	{
		ball->Update(timeTotal, timeDelta);
		paddle1->Update(timeTotal, timeDelta);
		paddle2->Update(timeTotal, timeDelta);
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
	ball->Draw(m_spriteBatch.get());
	paddle1->Draw(m_spriteBatch.get());
	paddle2->Draw(m_spriteBatch.get());

	float* stringlength = m_spriteFont->MeasureString(L"Tap to start!").m128_f32;

	if (!gameStarted)
	{
		m_spriteFont->DrawString(m_spriteBatch.get(), L"Tap to start!",
			XMFLOAT2(m_windowBounds.Width * scale / 2.0f, m_windowBounds.Height * scale / (5.0f / 2.0f)),
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

void Renderer::resetGame()
{
	gameStarted = false;
}

void Renderer::setGameRunning(bool running)
{
	gameStarted = running;
}

bool Renderer::getGameRunning()
{
	return gameStarted;
}