#pragma once
#include "SpriteBatch.h"
#include "Windows.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>

using namespace DirectX;

class Healthbar
{
public:
	Healthbar(XMFLOAT2 position, int totalHealth);

	void Update(XMFLOAT2 newPosition);
	void Draw(SpriteBatch* spriteBatch);

	void LoadTexture(ID3D11Device* d3dDevice);
	void decreaseHealth(int delta);
	void reset();
	bool hasNoHealth();
private:
	ID3D11ShaderResourceView* rectangle;
	int currentHealth, totalHealth;
	XMFLOAT2 size, position;
	bool noHealth;
};

