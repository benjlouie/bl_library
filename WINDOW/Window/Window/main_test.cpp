#include <iostream>
#include <list>
#include <SDL2\SDL.h>
#include "WindowRegion.h"

using namespace std;

int main(int argc, char **argv)
{
	int width = 640;
	int height = 480;

	string tmp;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	list<WindowRegion *> regions;
	list<WindowRegion *> freeList;
	WindowRegion *r1, *r2, *r3, *r4;
	r1 = new WindowRegion(renderer, 0.1F, 0.0F, 0.3F, 0.7F);
	r1->backgroundColor_ = SDL_Color{ 255, 0, 0, SDL_ALPHA_OPAQUE };
	regions.push_back(r1); freeList.push_back(r1);
	r2 = new WindowRegion(renderer, 0.423F, 0.4F, 0.233F, 0.5F);
	r2->backgroundColor_ = SDL_Color{ 0, 255, 0, SDL_ALPHA_OPAQUE };
	regions.push_back(r2); freeList.push_back(r2);
	r3 = new WindowRegion(renderer, 0.7F, 0.1F, 0.24F, 0.4F);
	r3->backgroundColor_ = SDL_Color{ 0, 0, 255, SDL_ALPHA_OPAQUE };
	regions.push_back(r3); freeList.push_back(r3);

	//subregion tests
	r4 = new WindowRegion(renderer, 0.25F, 0.25F, 0.5F, 0.5F);
	r4->backgroundColor_ = SDL_Color{ 0, 125, 120, SDL_ALPHA_OPAQUE };
	r3->addSubregion(r4); freeList.push_back(r4);

	r1 = new WindowRegion(renderer, 0.133F, 0.133F, 0.3F, 0.3F);
	r1->backgroundColor_ = SDL_Color{ 125, 125, 0, SDL_ALPHA_OPAQUE };
	r4->addSubregion(r1); freeList.push_back(r1);
	r1 = new WindowRegion(renderer, 0.566F, 0.133F, 0.3F, 0.3F);
	r1->backgroundColor_ = SDL_Color{ 125, 125, 0, SDL_ALPHA_OPAQUE };
	r4->addSubregion(r1); freeList.push_back(r1);
	r1 = new WindowRegion(renderer, 0.566F, 0.566F, 0.3F, 0.3F);
	r1->backgroundColor_ = SDL_Color{ 125, 125, 0, SDL_ALPHA_OPAQUE };
	r4->addSubregion(r1); freeList.push_back(r1);

	bool loop = true;
	while (loop) {
		//advanceTime()

		//handle events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
			else if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					width = event.window.data1;
					height = event.window.data2;

					for (WindowRegion *region : regions) {
						region->setDimentions(width, height, 0, 0);
					}
					break;
				default:
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		//render stuff
		for (WindowRegion *region : regions)
		{
			region->Render();
		}
		SDL_RenderPresent(renderer);


		SDL_Delay(16); //~60 per sec
	}

	for (WindowRegion *region : freeList) {
		delete region;
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

