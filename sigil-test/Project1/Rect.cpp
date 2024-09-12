#include "sl.h"
#include "Rect.h"
#include "Utils.h"

Rect RectSpace::GetDefaultRect()
{
	Rect myRect;
	myRect.width = 200;
	myRect.height = 10;
	myRect.pos.x = screenHalfWidth;
	myRect.pos.y = 0 + myRect.height;
	myRect.speed = 250.0f;
	return myRect;
}

void RectSpace::DrawRect(Rect myRect)
{
	slSetForeColor(0.8, 0.0, 0.2, 1.0);
	slRectangleFill(myRect.pos.x, myRect.pos.y, myRect.width, myRect.height);
}

void RectSpace::MoveRect(Rect& myRect)
{
	float currentSpeed = myRect.speed * slGetDeltaTime();

	if (slGetKey(SL_KEY_LEFT))
		myRect.pos.x -= currentSpeed;
	else if (slGetKey(SL_KEY_RIGHT))
		myRect.pos.x += currentSpeed;
}