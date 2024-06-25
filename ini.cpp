#include<SDL.h>
#include<stdio.h>
#include<string.h>
#include"structsanddefines.h"
#include"draw.h"

int ini_screen(SDL_Surface** screen, SDL_Texture** scrtex, SDL_Window** window, SDL_Renderer** renderer)
{
	int rc;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	// tryb pe³noekranowy / fullscreen mode
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	&(*window), &(*renderer));
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&(*window), &(*renderer));
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}

	*screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	*scrtex = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	return 0;
}
void getColor(SDL_Surface* background, Uint8* red, Uint8* green, Uint8* blue, int x, int y)
{
	SDL_PixelFormat* formt;
	Uint32 temp;
	Uint32 pixel;

	formt = background->format;


	SDL_LockSurface(background);

	/* Get pixel */
	pixel = *((Uint32*)background->pixels + (y*SCREEN_WIDTH + x));

	SDL_UnlockSurface(background);
	/*Red */
	temp = pixel & formt->Rmask;  /* Isolate red component */
	temp = temp >> formt->Rshift; /* Shift it down to 8-bit */
	temp = temp << formt->Rloss;  /* Expand to a full 8-bit number */
	*red = (Uint8)temp;

	/* Green */
	temp = pixel & formt->Gmask;  
	temp = temp >> formt->Gshift; 
	temp = temp << formt->Gloss;  
	*green = (Uint8)temp;

	/* Blue */
	temp = pixel & formt->Bmask;  
	temp = temp >> formt->Bshift; 
	temp = temp << formt->Bloss;  
	*blue = (Uint8)temp;
}

void calTime(int* t1, int t2,int* frames, double* delta, double* worldTime, double* respawnTimeally, double* respawnTimebullet, double* fpsTimer,double* fps, long long int* score, double speed, int* noPoints, double* moveTime, bool count)
{
	static double scorebuffer = 0;
	*delta = (t2 - *t1) * 0.001;
	*t1 = t2;
	*worldTime += *delta;
	*respawnTimeally += *delta;
	*respawnTimebullet += *delta;
	*moveTime += *delta;

	*fpsTimer += *delta;
	if (*fpsTimer > 0.5) {
		if (speed > 0 && *noPoints == 0 && count)
		{
			scorebuffer = scorebuffer + (*delta * speed) * 2;
			if (scorebuffer > 1)
			{
				*score = *score + (int)scorebuffer;
				scorebuffer = 0;
			}
		}
		else if(*noPoints > 0)
			(*noPoints)--;
		*fps = *frames * 2;
		*frames = 0;
		*fpsTimer -= 0.5;
	};
}

void calDistance(double* distance_d, int* distance, double delta, double speed, int* backgroundheight, car_t* player, car_t* ally, car_t* enemy, bullet_t* bullets, int ibullets)
{
	*distance_d = *distance_d + (delta * speed);
	*distance = (int)*distance_d;
	if (ally->x != 0)
	{
		ally->distance_delta = ally->distance_delta + (delta * ally->speed);
		int distance_ally = (int)ally->distance_delta;
		ally->y = ally->y + *distance -distance_ally;
		if (distance_ally)
			ally->distance_delta = 0;
	}

	if (enemy->x != 0)
	{
		enemy->distance_delta = enemy->distance_delta + (delta * enemy->speed);
		int distance_enemy = (int)enemy->distance_delta;
		enemy->y = enemy->y + *distance - distance_enemy;
		if (distance_enemy)
			enemy->distance_delta = 0;
	}

	for (int i = 0; i < ibullets; i++)
	{
		(bullets + i)->distance_delta = (bullets + i)->distance_delta + (delta *SPEEDBULLET);
		int distance_bullet = (int)(bullets + i)->distance_delta;
		(bullets + i)->y = (bullets + i)->y + *distance - distance_bullet;
		if (distance_bullet)
			(bullets + i)->distance_delta = 0;
	}
	
	if (*distance)
	{
		*backgroundheight = *backgroundheight + *distance;
		player->y = player->y - *distance;
		if (*backgroundheight > MAP_END)
			*backgroundheight = MAP_START + *distance;
		if (player->y < CAR_HEIGHT)
		{
			player->y = MAP_SIZE - 100 - *distance;
		}
		*distance = 0;
		*distance_d = 0;
	}
	
}

bool outofRoad(car_t player, SDL_Surface* background)
{
	Uint8 red, green, blue;
	int c = 0;
	for (int i = player.x - (HITBOX_WIDTH / 2); i < player.x + (HITBOX_WIDTH / 2); i++)
	{
		for (int j = player.y - (HITBOX_HEIGHT / 2); j < player.y + (HITBOX_HEIGHT / 2); j++)
		{
			if (j > 0)
			{
				getColor(background, &red, &green, &blue, i, j);
				if (red == 21 && green == 182 && blue == 4)
				{
					c++;
				}
			}
		}
	}

	if (c > 210)
		return true;
	else
		return false;
}

bool onRoad(car_t player, SDL_Surface* background)
{
	Uint8 red, green, blue;
	int c = 0;
	for (int i = player.x - (HITBOX_WIDTH / 2); i < player.x + (HITBOX_WIDTH / 2); i++)
	{
		for (int j = player.y - (HITBOX_HEIGHT / 2); j < player.y + (HITBOX_HEIGHT / 2); j++)
		{
			if (j > 0)
			{
				getColor(background, &red, &green, &blue, i, j);
				if (red == 47 && green == 43 && blue == 43)
				{
					c++;
				}
			}
		}
	}

	if (c > 210)
		return true;
	else
		return false;
}

bool onSide(car_t player, SDL_Surface* background)
{
	Uint8 red, green, blue;
	int c = 0;
	for (int i = player.x - (HITBOX_WIDTH / 2); i < player.x + (HITBOX_WIDTH / 2); i++)
	{
		for (int j = player.y - (HITBOX_HEIGHT / 2); j < player.y + (HITBOX_HEIGHT / 2); j++)
		{
			if (j > 0)
			{
				getColor(background, &red, &green, &blue, i, j);
				if (red == 0 && green == 0 && blue == 0)
				{
					c++;
				}
			}
		}
	}

	if (c > 10)
		return true;
	else
		return false;
}

void empty_buffer(char* buffer)
{
	for (int i = 0; i < 100; i++)
	{
		buffer[i] = '\0';
	}
}

void sort_by_points(score_t* tab, int i)
{
	for (int w = 0; w < i; w++)
	{
		for (int k = 1; k < i - w; k++)
		{
			if (tab[k - 1].score < tab[k].score)
			{
				score_t temp;
				temp.score = tab[k - 1].score;
				temp.time = tab[k - 1].time;
				tab[k - 1].score = tab[k].score;
				tab[k - 1].time = tab[k].time;
				tab[k].score = temp.score;
				tab[k].time = temp.time;
			}
		}
	}
}

void sort_by_time(score_t* tab, int i)
{
	for (int w = 0; w < i; w++)
	{
		for (int k = 1; k < i - w; k++)
		{
			if (tab[k - 1].time < tab[k].time)
			{
				score_t temp;
				temp.score = tab[k - 1].score;
				temp.time = tab[k - 1].time;
				tab[k - 1].score = tab[k].score;
				tab[k - 1].time = tab[k].time;
				tab[k].score = temp.score;
				tab[k].time = temp.time;
			}
		}
	}
}

void list(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer)
{
	int quit = 0, item = 0, l = 0;
	char text[128];
	char buffer[100];
	SDL_Event event;
	SDL_Surface* charset = NULL;
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 255, 5, 14);
	int niebieski = SDL_MapRGB(screen->format, 30, 00, 160);

	long long int score;
	double worldTime;

	FILE* file;

	charset = SDL_LoadBMP("cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);

	int i = 0;
	score_t* tab = NULL;

	fopen_s(&file, "list.txt", "r");
	if (file != NULL)
	{
		l = 0;
		empty_buffer(buffer);
		fgets(buffer, 100, file);
		while (l < 100)
		{
			if (buffer[l] >= '0' && buffer[l] <= '9')
			{
				i *= 10;
				i += (int)buffer[l] - 48;
			}
			else
				break;
			l++;
		}
		fclose(file);

		tab = (score_t*)malloc(i * sizeof(score_t));


		
		fopen_s(&file, "list.bin", "a+");
		if (file != NULL)
		{
			long long int tempscore = 0;
			double temptime = 0;
			for (int j = 0; j < i; j++)
			{
				l = 0;
				tempscore = 0;
				temptime = 0;
				empty_buffer(buffer);
				fgets(buffer, 100, file);
				while (l < 100)
				{
					if (buffer[l] >= '0' && buffer[l] <= '9')
					{
						tempscore *= 10;
						tempscore += (int)buffer[l] - 48;
					}
					else
						break;
					l++;
				}
				//printf("%d, %s\n", j, buffer);

				empty_buffer(buffer);
				fgets(buffer, 100, file);
				//printf("%s", buffer);
				l = 0;
				while (l < 100)
				{
					if (buffer[l] >= '0' && buffer[l] <= '9')
					{
						temptime *= 10;
						temptime += (int)buffer[l] - 48;
					}
					else if (buffer[l] == '.')
					{
						temptime += (int)(buffer[l + 1] - 48) / 10.0;
						break;
					}
					l++;
				}

				//printf("%d, %s\n", j, buffer);

				(tab + j)->score = tempscore;
				(tab + j)->time = temptime;

			}
			fclose(file);
		}

		while (!quit)
		{
			DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, czerwony, niebieski);

			sprintf_s(text, "ESC- quit to menu");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
			sprintf_s(text, "P- sort by points");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
			sprintf_s(text, "T- sort by time");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);

			sprintf_s(text, "   SCORE       TIME");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 60, text, charset);
			for (int j = 0; j < i && j < 20; j++)
			{
				sprintf_s(text, "%d.   ",j+1 + item);
				DrawString(screen, 320, 76 + j * 16, text, charset);
				sprintf_s(text, "  %lld   ", (tab + j + item)->score);
				DrawString(screen, 380, 76 + j * 16, text, charset);
				sprintf_s(text, "  %.1f  ", (tab + j + item)->time);
				DrawString(screen, 460, 76 + j * 16, text, charset);
			}

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_UP)
					{
						if (item != 0)
							item--;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (i - item > 20)
							item++;
					}
					else if (event.key.keysym.sym == SDLK_p)
					{
						sort_by_points(tab, i);
					}
					else if (event.key.keysym.sym == SDLK_t)
					{
						sort_by_time(tab, i);
					}
				}
			}

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

		}
		free(tab);
	}
}
