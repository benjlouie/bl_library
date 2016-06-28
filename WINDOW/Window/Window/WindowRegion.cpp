#include "WindowRegion.h"

WindowRegion::WindowRegion(SDL_Renderer *renderer, int x, int y, int w, int h)
{
	renderer_ = renderer;
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;

	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);
	parentWidth_ = width;
	parentHeight_ = height;
	xRel_ = (float)x / width;
	yRel_ = (float)y / height;
	wRel_ = (float)w / width;
	hRel_ = (float)h / height;
	
	xOffset_ = 0;
	yOffset_ = 0;
}

WindowRegion::WindowRegion(SDL_Renderer *renderer, float xRel, float yRel, float wRel, float hRel)
{
	renderer_ = renderer;
	xRel_ = xRel;
	yRel_ = yRel;
	wRel_ = wRel;
	hRel_ = hRel;

	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);
	parentWidth_ = width;
	parentHeight_ = height;
	x_ = (int)(xRel * width);
	y_ = (int)(yRel * height);
	w_ = (int)(wRel * width);
	h_ = (int)(hRel * height);

	xOffset_ = 0;
	yOffset_ = 0;
}

WindowRegion::WindowRegion(WindowRegion *parentRegion, float xRel, float yRel, float wRel, float hRel)
{
	renderer_ = parentRegion->renderer_;
	xRel_ = xRel;
	yRel_ = yRel;
	wRel_ = wRel;
	hRel_ = hRel;
	parentWidth_ = parentRegion->w_;
	parentHeight_ = parentRegion->h_;
	x_ = (int)(xRel * parentWidth_);
	y_ = (int)(yRel * parentHeight_);
	w_ = (int)(wRel * parentWidth_);
	h_ = (int)(hRel * parentHeight_);
	xOffset_ = parentRegion->x_;
	yOffset_ = parentRegion->y_;
}

void WindowRegion::addSubregion(WindowRegion *subregion)
{
	subregion->parentWidth_ = w_;
	subregion->parentHeight_ = h_;
	subregion->xOffset_ = xOffset_ + x_;
	subregion->yOffset_ = yOffset_ + y_;
	subregion->x_ = (int)(subregion->xRel_ * subregion->parentWidth_);
	subregion->y_ = (int)(subregion->yRel_ * subregion->parentHeight_);
	subregion->w_ = (int)(subregion->wRel_ * subregion->parentWidth_);
	subregion->h_ = (int)(subregion->hRel_ * subregion->parentHeight_);

	subregions.push_back(subregion);
}

//TODO: make work with subregions
void WindowRegion::setDimentions(int newWidth, int newHeight, int xOffset, int yOffset)
{
	parentWidth_ = newWidth;
	parentHeight_ = newHeight;

	xOffset_ = xOffset;
	yOffset_ = yOffset;
	x_ = (int)(xRel_ * parentWidth_);
	y_ = (int)(yRel_ * parentHeight_);
	w_ = (int)(wRel_ * parentWidth_);
	h_ = (int)(hRel_ * parentHeight_);

	for (WindowRegion *region : subregions) {
		region->setDimentions(w_, h_, xOffset_ + x_, yOffset_ + y_);
	}
}

void WindowRegion::Render(void)
{
	SDL_Rect region = { xOffset_ + x_, yOffset_ + y_, w_, h_ };
	SDL_SetRenderDrawColor(renderer_, backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a);
	SDL_RenderFillRect(renderer_, &region);

	//TODO: render stuff in this region
	for (WindowRegion *region : subregions) {
		region->Render();
	}
}