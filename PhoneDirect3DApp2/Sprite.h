#pragma once

#include "SpriteBatch.h"
#include "DDSTextureLoader.h"
#include <D3D11.h>
#include "Windows.h"
#include <list>
#include <vector>

using namespace DirectX;
using namespace std;

/*
The variables needed to properly manipulate this class outside of its internal methods are
animationState and shouldChangeAnimation.  These two must be changed in tandem to properly change the animation
of the sprite.  Furthermore, this class works under the assumption that the new animation is looping - that is,
the sprite will never switch to a different animation cycle automatically - it has to be triggered using the aforementioned variables.
Therefore, if you want an animation cycle to only run once, use the animationPlayedOnce variable to know when to change the loop back to something else.
*/
class Sprite
{
public:

	Sprite(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds, float scale = 1,
		float Speed = 0);
	//Sprite(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds, float scale);
	Sprite(ID3D11ShaderResourceView *m_Texture, XMFLOAT2 size, XMFLOAT2 position, Windows::Foundation::Rect* movementBounds, int rows, int columns,
		double framesPerSecond, int dividers[]);

	void Draw(SpriteBatch* spriteBatch);
	virtual void Update(float timeTotal, float timeDelta);

	float getWidth();
	float getHeight();
	XMFLOAT2 getVelocity();
	void setVelocity(XMFLOAT2 newVelocity);

private:
	XMFLOAT2 Velocity;
	int currentFrame;
	int animationState; //value that controls which part of the sprite sheet to animate (therefore, created sprites MUST modify this value)
	bool shouldChangeAnimation; //this value works in tandem with animationState
	int rows;
	int columns;
	int framesPerSecond;
	int totalFrames;
	double timeSinceLastFrame;
	float scale;

	Windows::Foundation::Rect* movementBounds;
	Windows::Foundation::Rect* BoundingBox;
	XMFLOAT2 position;
	XMFLOAT2 size;

	void NormalizeVelocity();
	Windows::Foundation::Rect* Sprite::CreateBoundingBoxFromPosition(XMFLOAT2 position);
	void UpdateAnimation(float timeTotal, float timeDelta);
	double SecondsBetweenFrames();

protected:
	bool animationPlayedOnce;
	int dividers[5];
	float Speed;
	ID3D11ShaderResourceView* m_Texture;

	bool Blocked(XMFLOAT2 newPosition);
};
