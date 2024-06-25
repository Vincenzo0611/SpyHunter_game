#include<SDL.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include"draw.h"
#include"ini.h"
#include"structsanddefines.h"
#include"game.h"


int main(int argc, char** argv) {

	int quit = 0, option = 0;
	SDL_Event event;
	SDL_Surface* screen = NULL, * charset = NULL;
	SDL_Texture* scrtex = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* menu;
	SDL_Surface* menu_selector;

	if (ini_screen(&screen, &scrtex, &window, &renderer))
	{
		return 1;
	}

	menu = SDL_LoadBMP("menu.bmp");
	menu_selector = SDL_LoadBMP("menu_selector.bmp");
	charset = SDL_LoadBMP("cs8x8.bmp");

	while (!quit)
	{


		DrawSurface(screen, menu, SCREEN_WIDTH / 2, SCREEN_HEIGHT/2);

		DrawSurface(screen, menu_selector, 420, (90 * option) + 195);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				if ((event.key.keysym.sym == SDLK_RETURN))
				{
					if (option == GAME_O)
						game(screen, scrtex, window, renderer, false);
					if(option == 1)
						game(screen, scrtex, window, renderer, true);
					if (option == 2)
						list(screen, scrtex, window, renderer);
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (option != 0)
						option--;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (option != MAX_OPTION)
						option++;
				}
			}
		}
	}

	
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	return 0;
}