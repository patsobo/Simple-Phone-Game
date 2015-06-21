#include "pch.h"
#include "Healthbar.h"


Healthbar::Healthbar(XMFLOAT2 position, int totalHealth)
{
	//this->rectangle = rectangle;
	this->totalHealth = totalHealth;
	this->currentHealth = totalHealth;
	//this->size = size;
	this->position = position;

	noHealth = false;
}

void Healthbar::Update(XMFLOAT2 newPosition)
{
	//position = newPosition;
}

void Healthbar::Draw(SpriteBatch* spriteBatch)
{
	float greenSizeX = (float(currentHealth) / totalHealth) * size.x;
	
	// Red
	RECT* sourceRectangle = new RECT;
	sourceRectangle->left = 0.0f;
	sourceRectangle->top = 0.0f;
	sourceRectangle->right = size.x;
	sourceRectangle->bottom = size.y;

	spriteBatch->Draw(rectangle, position, sourceRectangle, Colors::Red,
		0.0f, XMFLOAT2(0.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f), SpriteEffects_None, 0.0f);

	// Green
	sourceRectangle->right = greenSizeX;

	spriteBatch->Draw(rectangle, position,
		sourceRectangle, Colors::Green, 0.0f, XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f), SpriteEffects_None, 0.0f);
}

void Healthbar::decreaseHealth(int delta)
{
	currentHealth -= delta;
	if (currentHealth < 0)
		currentHealth = 0;
	if (currentHealth == 0)
		noHealth = true;
}

void Healthbar::LoadTexture(ID3D11Device* d3dDevice)
{
	CreateDDSTextureFromFile(d3dDevice,
		L"Assets/healthbar2.dds", nullptr, &rectangle, MAXSIZE_T);
	size = XMFLOAT2(100, 20);
}

bool Healthbar::hasNoHealth() { return noHealth; }
void Healthbar::reset()
{
	currentHealth = totalHealth; 
	noHealth = false;
}