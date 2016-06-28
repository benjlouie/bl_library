#include <vector>
#include <SDL2\SDL.h>

//TODO: remove debug stuff
#include <iostream>
#include <string>
#include <Windows.h>

//TODO: add stuff to render sub parts of the region

class WindowRegion {
public:
	SDL_Color backgroundColor_ = SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE };

	WindowRegion(SDL_Renderer *renderer, int x, int y, int w, int h);
	WindowRegion(SDL_Renderer *renderer, float xRel, float yRel, float wRel, float hRel);
	WindowRegion(WindowRegion *parentRegion, float xRel, float yRel, float wRel, float hRel);
	void addSubregion(WindowRegion *subRegion);
	void setDimentions(int newWidth, int newHeight, int xOffset, int yOffset);
	void Render(void);
	
	//TODO: make it so these aren't public
	int parentWidth_;
	int parentHeight_;
	int xOffset_;
	int yOffset_;
	int x_;
	int y_;
	int w_;
	int h_;
	//0 <= *Rel_ <= 1
	float xRel_;
	float yRel_;
	float wRel_;
	float hRel_;
protected:
	SDL_Renderer *renderer_;
	std::vector<WindowRegion *> subregions;

private:

};