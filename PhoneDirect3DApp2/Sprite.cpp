#include "pch.h"
#include "Sprite.h"

using namespace DirectX;
using namespace std;

Sprite::Sprite(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds) :
m_Texture(m_Texture), size(size), position(position), movementBounds(movementBounds), rows(1), columns(1), framesPerSecond(1)
{
	this->position = position;
	this->movementBounds = movementBounds;
	this->m_Texture = m_Texture;
	this->size = size;

	for (int i = 0; i < 5; i++)
	{
		this->dividers[i] = 0;
	}

	animationState = 0;
	currentFrame = 0;
	totalFrames = rows*columns;
	BoundingBox = CreateBoundingBoxFromPosition(Sprite::position);
	shouldChangeAnimation = false;
	timeSinceLastFrame = 0;
}

Sprite::Sprite(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds, int rows, int columns,
	double framesPerSecond, int dividers[])
{
	this->position = position;
	this->movementBounds = movementBounds;
	this->rows = rows;
	this->columns = columns;
	this->framesPerSecond = framesPerSecond;

	for (int i = 0; i < 5; i++)
		this->dividers[i] = dividers[i];

	this->m_Texture = m_Texture;
	this->size = size;

	animationState = 0;
	currentFrame = 0;
	totalFrames = rows*columns;
	BoundingBox = CreateBoundingBoxFromPosition(this->position);
	shouldChangeAnimation = false;
	timeSinceLastFrame = 0;
}

void Sprite::Draw(SpriteBatch* spriteBatch)
{
	double imageWidth = size.x / columns;
	double imageHeight = size.y / rows;

	int currentRow = currentFrame / columns;
	int currentColumn = currentFrame%columns;

	RECT* sourceRectangle = new RECT;
	sourceRectangle->left = imageWidth*currentColumn;
	sourceRectangle->top = imageHeight*currentRow;
	sourceRectangle->right = imageWidth*currentColumn + imageWidth;
	sourceRectangle->bottom = imageHeight*currentRow + imageHeight;

	spriteBatch->Draw(m_Texture, XMFLOAT2(position.x, position.y),
		sourceRectangle, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(getWidth() / imageWidth, getHeight() / imageHeight),
		DirectX::SpriteEffects_None, 0.0f);
}

void Sprite::Update(float timeTotal, float timeDelta)
{
	UpdateAnimation(timeTotal, timeDelta);

	XMFLOAT2 newPosition;
	newPosition.x = position.x + ((Velocity.x * timeDelta) * Speed);
	newPosition.y = position.y + ((Velocity.y * timeDelta) * Speed);

	if (Blocked(newPosition))
		return;

	position = newPosition;
}

void Sprite::UpdateAnimation(float timeTotal, float timeDelta)
{
	timeSinceLastFrame += timeDelta;

	if (timeSinceLastFrame > SecondsBetweenFrames() && !shouldChangeAnimation)	//continue animating as normal
	{
		currentFrame++;
		timeSinceLastFrame = 0;
	}

	//else if (shouldChangeAnimation && inSecondAnimation)	//if should change out of second animation, go to first animation
	//{
	//	currentFrame = 0;
	//	timeSinceLastFrame = 0;
	//	shouldChangeAnimation = false;
	//}

	else if (shouldChangeAnimation)	//go to specified animation
	{
		currentFrame = dividers[animationState];
		timeSinceLastFrame = 0;
		shouldChangeAnimation = false;
		animationPlayedOnce = false;
	}

	//looping conditional statements
	if (currentFrame == dividers[animationState + 1] - 1 && dividers[animationState + 1] != 0)	//if you reach the end of your animation, loop that animation again
	{
		currentFrame = dividers[animationState];
		animationPlayedOnce = true;
	}

	else if (currentFrame == totalFrames)	//if at the end of sprite sheet, loop animation again
	{
		currentFrame = dividers[animationState];
		timeSinceLastFrame = 0;
		animationPlayedOnce = true;
	}
}

double Sprite::SecondsBetweenFrames()
{
	return 1.0 / framesPerSecond;
}

bool Sprite::Blocked(XMFLOAT2 newPosition)
{
	Windows::Foundation::Rect* boundingBox = CreateBoundingBoxFromPosition(newPosition);

	return !movementBounds->IntersectsWith(*boundingBox);
}

Windows::Foundation::Rect* Sprite::CreateBoundingBoxFromPosition(XMFLOAT2 position)
{
	return new Windows::Foundation::Rect(position.x, position.y, getWidth(), getHeight());
}

float Sprite::getWidth() { return size.x / columns; }
float Sprite::getHeight() { return size.y / rows; }




