#include<SDL.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"structsanddefines.h"
#include"ini.h"
#include"draw.h"


int placeforcar(car_t player, SDL_Surface* background)
{
	car_t test;
	test.y = player.y +120 - SCREEN_HEIGHT;
	if (test.y < (HITBOX_HEIGHT / 2))
	{
		test.y = MAP_SIZE - test.y;
	}
	for (int i = 0; i < 100; i++)
	{
		test.x = rand() % SCREEN_WIDTH;
		if ((test.x < player.x - HITBOX_WIDTH || test.x > player.x + HITBOX_WIDTH) && onRoad(test, background))
			break;
		else
			test.x = 0;
	}
	return test.x;

}

void start_new_game(car_t* player, int* backgroundheight, int* distance, double* distance_d, double* speed, long long int* score, double*  worldTime, double* respawnTimeally, double* respawnTimebullet, car_t* ally, car_t* enemy, int* noPoints)
{
	player->x = CAR_START_WIDTH;
	player->y = MAP_SIZE - 100;
	*backgroundheight = MAP_START;
	*distance = 0;
	*distance_d = 0;
	*speed = 0;
	*score = 0;
	*worldTime = 0;
	*respawnTimeally = 0;
	ally->x = 0;
	ally->y = 0;
	ally->set = false;

	enemy->y = 0;
	enemy->x = 0;
	enemy->set = false;
	respawnTimebullet = 0;
	enemy->distance_delta = 0;
	enemy->direction = 0;

	*noPoints = 0;
}

int movecar(car_t* bot, SDL_Surface* background, car_t player)
{
	int result = 0;
	car_t test;
	test.x = bot->x;
	test.y = player.y + 100 - SCREEN_HEIGHT + bot->y - 60;
	if (test.y < (HITBOX_HEIGHT / 2))
	{
		test.y = MAP_SIZE - test.y;
	}
	if (!onRoad(test, background))
	{
		for (int i = 0; i < SCREEN_WIDTH / 2; i++)
		{
			if(bot->x - i > 0)
			{
				test.x = bot->x - i;
				if (onRoad(test, background))
				{
					bot->direction = test.x;
					result = 1;
					break;
				}
			}
			if (bot->x + i < SCREEN_WIDTH)
			{
				test.x = bot->x + i;
				if (onRoad(test, background))
				{
					bot->direction = test.x;
					result = 1;
					break;
				}
			}
		}
	}
	return result;
}

int typeofColision(car_t player, car_t bot)
{
	int topPlayer = SCREEN_HEIGHT - 100 - HITBOX_HEIGHT / 2;
	int bottomPlayer = SCREEN_HEIGHT - 100 + HITBOX_HEIGHT / 2;
	int leftPlayer = player.x - HITBOX_WIDTH / 2;
	int rightPlayer = player.x + HITBOX_WIDTH / 2;

	int topBot = bot.y - HITBOX_HEIGHT / 2;
	int bottomBot = bot.y + HITBOX_HEIGHT / 2;
	int leftBot = bot.x - HITBOX_WIDTH / 2;
	int rightBot = bot.x + HITBOX_WIDTH / 2;

	//przod
	if (bottomBot - topPlayer >= 0 && bottomBot - topPlayer <= HITBOX_HEIGHT/2)
	{
		if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightPlayer - leftBot)
				return 1; //przodem
			else
				return 3; //uderzenie prawym bokiem
		}
		else if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightBot - leftPlayer)
				return 1; //przodem
			else
				return 4; //uderzenie lewym bokiem
		}
		else if ((leftBot >= leftPlayer && leftBot <= rightPlayer) || (rightBot <= rightPlayer && rightBot >= leftPlayer))
			return 1; //przodem
	}

	//tyl
	if (bottomPlayer - topBot >= 0 && bottomPlayer - topBot <= HITBOX_HEIGHT / 2)
	{
		if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightPlayer - leftBot)
				return 2; //tylem
			else
				return 3; //uderzenie prawym bokiem
		}
		else if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightBot - leftPlayer)
				return 2; //tylem
			else
				return 4; //uderzenie lewym bokiem
		}
		else if ((leftBot >= leftPlayer && leftBot <= rightPlayer) || (rightBot <= rightPlayer && rightBot >= leftPlayer))
			return 2; //tylem
	}

	//prawo
	
	if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH/2)
	{
		if ((topBot >= topPlayer && topBot <= bottomPlayer) || (bottomBot <= bottomPlayer && bottomBot >= topPlayer))
			return 3; //prawo
	}

	//lewo
	if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
	{
		if ((topBot >= topPlayer && topBot <= bottomPlayer) || (bottomBot <= bottomPlayer && bottomBot >= topPlayer))
			return 4; //lewo
	}

	return 0;
}

int typeofColision_bots(car_t enemy, car_t bot)
{
	int topPlayer = enemy.y - HITBOX_HEIGHT / 2;
	int bottomPlayer = enemy.y + HITBOX_HEIGHT / 2;
	int leftPlayer = enemy.x - HITBOX_WIDTH / 2;
	int rightPlayer = enemy.x + HITBOX_WIDTH / 2;

	int topBot = bot.y - HITBOX_HEIGHT / 2;
	int bottomBot = bot.y + HITBOX_HEIGHT / 2;
	int leftBot = bot.x - HITBOX_WIDTH / 2;
	int rightBot = bot.x + HITBOX_WIDTH / 2;

	//przod
	if (bottomBot - topPlayer >= 0 && bottomBot - topPlayer <= HITBOX_HEIGHT / 2)
	{
		if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightPlayer - leftBot)
				return 1; //przodem
			else
				return 3; //uderzenie prawym bokiem
		}
		else if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightBot - leftPlayer)
				return 1; //przodem
			else
				return 4; //uderzenie lewym bokiem
		}
		else if ((leftBot >= leftPlayer && leftBot <= rightPlayer) || (rightBot <= rightPlayer && rightBot >= leftPlayer))
			return 1; //przodem
	}

	//tyl
	if (bottomPlayer - topBot >= 0 && bottomPlayer - topBot <= HITBOX_HEIGHT / 2)
	{
		if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightPlayer - leftBot)
				return 2; //tylem
			else
				return 3; //uderzenie prawym bokiem
		}
		else if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
		{
			if (bottomBot - topPlayer <= rightBot - leftPlayer)
				return 2; //tylem
			else
				return 4; //uderzenie lewym bokiem
		}
		else if ((leftBot >= leftPlayer && leftBot <= rightPlayer) || (rightBot <= rightPlayer && rightBot >= leftPlayer))
			return 2; //tylem
	}

	//prawo

	if (rightPlayer - leftBot >= 0 && rightPlayer - leftBot <= HITBOX_WIDTH / 2)
	{
		if ((topBot >= topPlayer && topBot <= bottomPlayer) || (bottomBot <= bottomPlayer && bottomBot >= topPlayer))
			return 3; //prawo
	}

	//lewo
	if (rightBot - leftPlayer >= 0 && rightBot - leftPlayer <= HITBOX_WIDTH / 2)
	{
		if ((topBot >= topPlayer && topBot <= bottomPlayer) || (bottomBot <= bottomPlayer && bottomBot >= topPlayer))
			return 4; //lewo
	}

	return 0;
}
void save(car_t *player, car_t *ally, car_t *enemy, double *worldTime, long long int *score, bullet_t* bullets, int *backgroundheight, int* ibullets)
{
	FILE* saves;
	fopen_s(&saves, "zapisy.txt", "a+");
	if (saves != NULL)
	{
		time_t rawtime;
		struct tm timeinfo;
		char file[30];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		//asctime_s(godzina, 30, &timeinfo);
		file[0] = 's';
		file[1] = 'a';
		file[2] = 'v';
		file[3] = 'e';
		file[4] = 's';
		file[5] = '\\';
		file[6] = (timeinfo.tm_mday / 10) + 48;
		file[7] = timeinfo.tm_mday % 10 + 48;
		file[8] = '_';
		file[9] = (timeinfo.tm_mon / 10) + 48;
		file[10] = timeinfo.tm_mon % 10 + +49;
		file[11] = '_';
		file[12] = (timeinfo.tm_year+1900) / 1000 + 48;
		file[13] = (timeinfo.tm_year + 1900) / 100 % 10 + 48;
		file[14] = (timeinfo.tm_year + 1900) / 10 % 10 + 48;
		file[15] = (timeinfo.tm_year + 1900) % 10 + 48;
		file[16] = '_';
		file[17] = (timeinfo.tm_hour / 10) + 48;
		file[18] = timeinfo.tm_hour % 10 + 48;
		file[19] = '_';
		file[20] = (timeinfo.tm_min / 10) + 48;
		file[21] = timeinfo.tm_min % 10 + 48;
		file[22] = '_';
		file[23] = (timeinfo.tm_sec / 10) + 48;
		file[24] = timeinfo.tm_sec % 10 + 48;
		file[25] = '.';
		file[26] = 'b';
		file[27] = 'i';
		file[28] = 'n';
		file[29] = '\0';
		
		//printf("%s", file);
		
		for (int i = 0; i < 29; i++)
		{
			fputc(file[i], saves);
		}

		fputc('\n', saves);

		FILE* save;
		fopen_s(&save, file, "w");
		if (save != NULL)
		{
			fwrite(player, sizeof(car_t), 1, save);
			fwrite(enemy, sizeof(car_t), 1, save);
			fwrite(ally, sizeof(car_t), 1, save);
			fwrite(backgroundheight, sizeof(int), 1, save);
			fwrite(ibullets, sizeof(int), 1, save);
			fwrite(worldTime, sizeof(double), 1, save);
			fwrite(score, sizeof(long long int), 1, save);
			fwrite(bullets, sizeof(bullet_t), 20, save);
			fclose(save);
		}
		fclose(saves);
	}
}

void load(car_t* player, car_t* ally, car_t* enemy, double* worldTime, long long int* score, bullet_t* bullets, int* backgroundheight, int* ibullets, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset)
{
	int option = 0, numberofitem = 0, numberofprint = 0;
	int quit = 0;
	bool loaded = 0;
	char text[128];
	SDL_Event event;
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 255, 5, 14);
	int niebieski = SDL_MapRGB(screen->format, 30, 00, 160);

	FILE* saves;
	char file[30];
	while (!quit) 
	{
		int i = 0;
		fopen_s(&saves, "zapisy.txt", "a+");
		if (saves != NULL)
		{
			if (loaded)
			{
				while (fgets(file, 30, saves))
				{
					if (option == i)
					{
						FILE* save;
						fopen_s(&save, file, "r");
						if (save != NULL)
						{
							fread(player, sizeof(car_t), 1, save);
							fread(enemy, sizeof(car_t), 1, save);
							fread(ally, sizeof(car_t), 1, save);
							fread(backgroundheight, sizeof(int), 1, save);
							fread(ibullets, sizeof(int), 1, save);
							fread(worldTime, sizeof(double), 1, save);
							fread(score, sizeof(long long int), 1, save);
							fread(bullets, sizeof(bullet_t), 20, save);
							fclose(save);
							quit = 1;
							break;
						}
					}
					fgets(file, 4, saves);
					i++;
				}
			}
			fclose(saves);
		}
		i = 0;
		fopen_s(&saves, "zapisy.txt", "a+");
		if (saves != NULL)
		{
			DrawRectangle(screen, 60, 60, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120, czerwony, niebieski);
			DrawRectangle(screen, 80, 66 + numberofitem*16, SCREEN_WIDTH - 160, 10, czerwony, niebieski);
			numberofprint = 0;
			while (fgets(file, 30, saves))
			{
				if (option - numberofprint <= 21 && i <= 21)
				{
					sprintf_s(text, file);
					DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 66 + i * 16, text, charset);
					i++;
				}
				fgets(file, 4, saves);
				numberofprint++;
			}

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			fclose(saves);

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_UP) {
						if (option != 0)
						{
							if (numberofitem != 0 && option < 22)
								numberofitem--;
							option--;
						}
					}
					else if (event.key.keysym.sym == SDLK_DOWN) {
						if (option < numberofprint - 1)
						{
							option++;
							if (numberofitem != 21)
								numberofitem++;
						}
					}
					else if (event.key.keysym.sym == SDLK_RETURN) {
						loaded = true;
					}
				}
			}
		}
	}
}


int pause(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset)
{
	char text[128];
	int quit = 0;
	SDL_Event event;

	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 255, 5, 14);
	int niebieski = SDL_MapRGB(screen->format, 30, 00, 160);

	while (!quit)
	{
		DrawRectangle(screen, 60, 60, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120, czerwony, niebieski);
		sprintf_s(text, "GAME PAUSED");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 66, text, charset);
		sprintf_s(text, "P - resume game");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 82, text, charset);
		sprintf_s(text, "N- start new game");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 98, text, charset);
		sprintf_s(text, "L- load game");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 114, text, charset);
		sprintf_s(text, "S- save game");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 130, text, charset);
		sprintf_s(text, "ESC- quit to menu");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 146, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return 1;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) return 1;
				else if (event.key.keysym.sym == SDLK_p) return 0;
				else if (event.key.keysym.sym == SDLK_n) return 2;
				else if (event.key.keysym.sym == SDLK_s) return 3;
				else if (event.key.keysym.sym == SDLK_l) return 4;
			}
		}
	}
}

int game_over(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset, long long int score, double worldTime)
{
	char text[128];
	int quit = 0;
	SDL_Event event;

	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 255, 5, 14);
	int niebieski = SDL_MapRGB(screen->format, 30, 00, 160);

	while (!quit)
	{
		DrawRectangle(screen, 60, 60, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120, czerwony, niebieski);
		sprintf_s(text, " GAME OVER");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 180, text, charset);
		sprintf_s(text, "TIME: %.1lfs            SCORE: %lld", worldTime, score);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 215, text, charset);
		sprintf_s(text, "N- start new game");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 250, text, charset);
		sprintf_s(text, "ESC- quit to menu");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 266, text, charset);
		sprintf_s(text, "X- save score and quit to menu");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 282, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return 1;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) return 1;
				else if (event.key.keysym.sym == SDLK_n) return 2;
				else if (event.key.keysym.sym == SDLK_x) return 3;
			}
		}
	}
}
void createbullet(bullet_t* bullets, car_t player, int* ibullets)
{
	(bullets + *ibullets)->x = player.x;
	(bullets + *ibullets)->y = CAR_HEIGHT - 30;
	(bullets + *ibullets)->distance_delta = 0;
	(*ibullets)++;
}
void deletebullet(bullet_t* bullets, int* ibullets, int deleteindex)
{
	for (int i = deleteindex + 1; i < *ibullets; i++)
	{
		(bullets + i - 1)->x = (bullets + i)->x;
		(bullets + i - 1)->y = (bullets + i)->y;
		(bullets + i - 1)->distance_delta = (bullets + i)->distance_delta;
	}
	(*ibullets)--;
}

bool colisionbullets(bullet_t* bullets, car_t bot, int* ibullets)
{
	int topBot = bot.y - HITBOX_HEIGHT / 2;
	int bottomBot = bot.y + HITBOX_HEIGHT / 2;
	int leftBot = bot.x - HITBOX_WIDTH / 2;
	int rightBot = bot.x + HITBOX_WIDTH / 2;

	for (int i = 0; i < *ibullets; i++)
	{
		int topBullet = (bullets + i)->y - HITBOXBULLET_HEIGHT / 2;
		int bottomBullet = (bullets + i)->y -+ HITBOXBULLET_HEIGHT / 2;
		int leftBullet = (bullets + i)->x - HITBOXBULLET_WIDTH / 2;
		int rightBullet = (bullets + i)->x + HITBOXBULLET_WIDTH / 2;

		if ((topBullet <= bottomBot && bottomBullet >= topBot) && ( (leftBullet >= leftBot && leftBullet <= rightBot) || (rightBullet <= rightBot && rightBullet >= leftBot)))
		{
			deletebullet(bullets, ibullets, i);
			return true;
		}
	}

	return false;
}

void save_score(double worldTime, long long int score)
{
	FILE* file;
	int i = 0;
	score_t* tab = NULL;
	score_t* tabtmp = NULL;

	fopen_s(&file, "list.txt", "r");
	if (file != NULL)
	{
		char buffer[100];
		int l = 0;
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

		i++;

		fclose(file);

		fopen_s(&file, "list.txt", "w");
		if (file != NULL)
		{
			fprintf(file, "%d\n", i);
			fclose(file);
		}

		fopen_s(&file, "list.bin", "a+");
		if (file != NULL)
		{
			fprintf(file, "%lld\n", score);
			fprintf(file, "%.1f\n", worldTime);
			fclose(file);
		}
	}
	else
	{
		i = 1;
		fopen_s(&file, "list.txt", "w");
		if (file != NULL)
		{
			fwrite(&i, sizeof(int), 1, file);
			fclose(file);
		}
		fopen_s(&file, "list.bin", "a+");
		if (file != NULL)
		{
			fprintf(file, "%lld\n", score);
			fprintf(file, "%.1f\n", worldTime);
			fclose(file);
		}
	}
}

void game(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, bool loaded)
{
	int quit = 0, rc = 0, backgroundheight = MAP_START, distance = 0, colision = 0 , noPoints = 0, ibullets = 0, start = 0;
	long long int score = 0;
	int t1, t2, frames = 0;
	double delta, worldTime = 0, fpsTimer = 0, fps = 0, distance_d = 0, respawnTimeally= 0, respawnTimebullet = 0, moveTime = 0;
	bool count = true;
	SDL_Event event;
	SDL_Surface* charset = NULL;
	SDL_Surface* background;
	SDL_Surface* car;
	SDL_Surface* car_ally;
	SDL_Surface* car_enemy;
	SDL_Surface* bullet;
	car_t player, ally, enemy;
	bullet_t* bullets;

	srand((unsigned int)time(NULL));

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 255, 5, 14);
	int niebieski = SDL_MapRGB(screen->format, 30, 00, 160);

	player.x = CAR_START_WIDTH;
	player.y = MAP_SIZE - 100;
	player.set = true;
	player.speed = 0;
	player.distance_delta = 0;
	player.direction = 0;
	ally.x = 0;
	ally.y = 0;
	ally.set = false;
	ally.distance_delta = 0;
	ally.speed = 200;
	ally.direction = 0;
	enemy.x = 0;
	enemy.y = 0;
	enemy.set = false;
	enemy.distance_delta = 0;
	enemy.speed = 240;
	enemy.direction = 0;

	bullets = (bullet_t*)malloc(BULLETSONSCREEN * sizeof(bullet_t));

	if (bullets == NULL)
		quit = 1;

	background = SDL_LoadBMP("background.bmp");
	car = SDL_LoadBMP("car.bmp");
	car_ally = SDL_LoadBMP("car_ally.bmp");
	car_enemy = SDL_LoadBMP("car_enemy.bmp");
	bullet = SDL_LoadBMP("bullet.bmp");
	charset = SDL_LoadBMP("cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);
	if (loaded)
	{
		load(&player, &ally, &enemy, &worldTime, &score, bullets, &backgroundheight, &ibullets, screen, scrtex, window, renderer, charset);
		start = 0;
		while (!start)
		{
			while (SDL_PollEvent(&event)) {
				DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, czerwony, niebieski);
				sprintf_s(text, "Game loaded");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 221, text, charset);
				sprintf_s(text, "Click ENTER to resume game");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 237, text, charset);
				SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, scrtex, NULL, NULL);
				SDL_RenderPresent(renderer);
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_RETURN) start = 1;
					break;
				}
			}
		}
	}
	t1 = SDL_GetTicks();

	while (!quit)
	{
		t2 = SDL_GetTicks();
		frames++;
		calTime(&t1, t2, &frames, &delta, &worldTime, &respawnTimeally, &respawnTimebullet, &fpsTimer, &fps, &score, player.speed, &noPoints, &moveTime, count);

		calDistance(&distance_d, &distance, delta, player.speed, &backgroundheight, &player, &ally, &enemy, bullets, ibullets);

		DrawSurface(screen, background, SCREEN_WIDTH / 2, backgroundheight);
		DrawSurface(screen, car, player.x, CAR_HEIGHT);

		if (!ally.set && respawnTimeally > 2)
		{
			int x = 0;
			respawnTimeally = 0;
			x = placeforcar(player, background);
			if (x)
			{
				ally.x = x;
				ally.y = 20;
				ally.set = true;
				ally.speed = 200;
			}
		}
		else if (ally.set)
		{
			car_t test;
			test.x = ally.x;
			test.y = player.y + 100 - SCREEN_HEIGHT + ally.y;
			if (onSide(test, background) && ally.speed > 100)
			{
				ally.speed = 100;
			}
			if (outofRoad(test, background) || colisionbullets(bullets, ally, &ibullets))
			{
				ally.y = 0;
				ally.x = 0;
				ally.set = false;
				respawnTimeally = 0;
				ally.distance_delta = 0;
				ally.direction = 0;
				noPoints = NOPOINTS_TIME;
			}
			if (ally.y > SCREEN_HEIGHT || ally.y < 0)
			{
				ally.y = 0;
				ally.x = 0;
				ally.set = false;
				respawnTimeally = 0;
				ally.distance_delta = 0;
				ally.direction = 0;
			}
			else
				movecar(&ally, background, player);
			if (ally.direction != ally.x && ally.direction != 0)
			{
				if (ally.direction > ally.x)
					ally.x += 1;
				else
					ally.x -= 1;
			}
			
			DrawSurface(screen, car_ally, ally.x, ally.y);

			colision = typeofColision(player, ally);

			switch (colision)
			{
				case 1: //przod
					switch (game_over(screen, scrtex, window, renderer, charset, score, worldTime))
					{
					case 1:
						quit = 1;
						break;
					case 2:
						start_new_game(&player, &backgroundheight, &distance, &distance_d, &player.speed, &score, &worldTime, &respawnTimeally, &respawnTimebullet, &ally, &enemy, &noPoints);
						break;
					case 3:
						save_score(worldTime, score);
						quit = 1;
						break;
					}
					break;
				case 2: //tyl
					ally.y = 0;
					ally.x = 0;
					ally.set = false;
					respawnTimeally = 0;
					ally.distance_delta = 0;
					ally.direction = 0;
					noPoints = NOPOINTS_TIME;
					break;
				case 3: //prawo
					ally.x += 3;
					break;
				case 4: //lewo
					ally.x -= 3;
			}
		}

		//enemy

		if (!enemy.set)
		{
			int x = 0;
			x = placeforcar(player, background);
			if (x)
			{
				enemy.x = x;
				enemy.y = 20;
				enemy.set = true;
				enemy.speed = 240;
			}
		}
		else if (enemy.set)
		{
			car_t test;
			test.x = enemy.x;
			test.y = player.y + 100 - SCREEN_HEIGHT + enemy.y;
			if (rand() % 20 == 0)
			{
				enemy.direction = player.x;
				if (player.speed >0 && player.speed < 100)
					enemy.speed = player.speed - 20;
			}
			if (onSide(test, background) && enemy.speed > 100)
			{
				enemy.speed = 100;
			}
			if (outofRoad(test, background) || colisionbullets(bullets, enemy, &ibullets))
			{
				enemy.y = 0;
				enemy.x = 0;
				enemy.set = false;
				enemy.distance_delta = 0;
				enemy.direction = 0;
				score += 5;
			}
			if (enemy.y > SCREEN_HEIGHT || enemy.y < 0)
			{
				enemy.y = 0;
				enemy.x = 0;
				enemy.set = false;
				enemy.distance_delta = 0;
				enemy.direction = 0;
			}
			else if (movecar(&enemy, background, player))
			{
				if (enemy.direction != enemy.x && enemy.direction != 0)
				{
					if (enemy.direction > enemy.x)
						enemy.x += 1;
					else
						enemy.x -= 1;
				}
			}
			else
			{
				if (enemy.direction != enemy.x && enemy.direction != 0 && moveTime > 0.01)
				{
					moveTime = 0;
					if (enemy.direction > enemy.x)
						enemy.x += 1;
					else
						enemy.x -= 1;
				
				}
			}

			DrawSurface(screen, car_enemy, enemy.x, enemy.y);

			colision = typeofColision(player, enemy);

			switch (colision)
			{
			case 1: //przod
				switch (game_over(screen, scrtex, window, renderer, charset, score, worldTime))
				{
				case 1:
					quit = 1;
					break;
				case 2:
					start_new_game(&player, &backgroundheight, &distance, &distance_d, &player.speed, &score, &worldTime, &respawnTimeally, &respawnTimebullet, &ally, &enemy, &noPoints);
					break;
				case 3:
					save_score(worldTime, score);
					quit = 1;
					break;
				}
				break;
			case 2: //tyl
				enemy.y = 0;
				enemy.x = 0;
				enemy.set = false;
				enemy.distance_delta = 0;
				enemy.direction = 0;
				score += 5;
				break;
			case 3: //prawo
				enemy.x += 3;
				break;
			case 4: //lewo
				enemy.x -= 3;
			}

			if(ally.set)
			{
				colision = typeofColision_bots(enemy, ally);

				switch (colision)
				{
				case 1: //przod
					enemy.y = 0;
					enemy.x = 0;
					enemy.set = false;
					enemy.distance_delta = 0;
					enemy.direction = 0;
					break;
				case 2: //tyl
					ally.y = 0;
					ally.x = 0;
					ally.set = false;
					respawnTimeally = 0;
					ally.distance_delta = 0;
					ally.direction = 0;
					break;
				case 3: //prawo
					ally.x += 3;
					break;
				case 4: //lewo
					ally.x -= 3;
				}
			}
		}

		//bullets
		if (ibullets > 0)
		{
			for (int i = 0; i < ibullets; i++)
			{
				DrawSurface(screen, bullet, (bullets + i)->x, (bullets + i)->y);
				if ((bullets + i)->y < RANGEBULLET)
				{
					deletebullet(bullets, &ibullets, 0);
				}
			}
		}



		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 52, czerwony, niebieski);
		sprintf_s(text, "VINCENZO PIRAS 193218");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		sprintf_s(text, "TIME: %.1lf s       FPS: %.0lf       SPEED: %.1lf px/s", worldTime, fps, player.speed);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
		sprintf_s(text, "SCORE: %lld", score);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);



		DrawRectangle(screen, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60, 60, 60, czerwony, niebieski);
		sprintf_s(text, "ABCDEF");
		DrawString(screen, SCREEN_WIDTH - 30 - strlen(text) * 8 / 2, SCREEN_HEIGHT - 60 + 11, text, charset);
		sprintf_s(text, "GHIJKN");
		DrawString(screen, SCREEN_WIDTH - 30 - strlen(text) * 8 / 2, SCREEN_HEIGHT - 30 + 11, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (outofRoad(player, background))
		{
			switch (game_over(screen, scrtex, window, renderer, charset, score, worldTime))
			{
			case 1:
				quit = 1;
				break;
			case 2:
				start_new_game(&player, &backgroundheight, &distance, &distance_d, &player.speed, &score, &worldTime, &respawnTimeally, &respawnTimebullet, &ally, &enemy, &noPoints);
				break;
			case 3:
				save_score(worldTime, score);
				quit = 1;
				break;
			}
		}
		if (onSide(player, background) && player.speed > 100)
		{
			if (player.speed > 100)
				player.speed -= 1;
			count = false;
		}
		else
			count = true;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (player.speed < MAX_SPEED)
						player.speed = player.speed + 20;
					if (player.speed > MAX_SPEED)
						player.speed = MAX_SPEED;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (player.speed > 0)
					{
						player.speed = player.speed - 20;
						if (player.speed < 0)
							player.speed = 0;
					}

				}
				else if (event.key.keysym.sym == SDLK_LEFT)  player.x = player.x - 5;
				else if (event.key.keysym.sym == SDLK_RIGHT)  player.x = player.x + 5;
				else if (event.key.keysym.sym == SDLK_n)
					start_new_game(&player, &backgroundheight, &distance, &distance_d, &player.speed, &score, &worldTime, &respawnTimeally, &respawnTimebullet, &ally, &enemy, &noPoints);
				else if (event.key.keysym.sym == SDLK_p)
				{
					switch (pause(screen, scrtex, window, renderer, charset))
					{
					case 1:
						quit = 1;
						break;
					case 2:
						start_new_game(&player, &backgroundheight, &distance, &distance_d, &player.speed, &score, &worldTime, &respawnTimeally, &respawnTimebullet, &ally, &enemy, &noPoints);
						break;
					case 3:
						save(&player, &ally, &enemy, &worldTime, &score, bullets, &backgroundheight, &ibullets);
						start = 0;
						while (!start)
						{
							while (SDL_PollEvent(&event)) {
								DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, czerwony, niebieski);
								sprintf_s(text, "Game saved");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 221, text, charset);
								sprintf_s(text, "Click ENTER to resume game");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 237, text, charset);
								SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
								SDL_RenderClear(renderer);
								SDL_RenderCopy(renderer, scrtex, NULL, NULL);
								SDL_RenderPresent(renderer);
								switch (event.type) {
								case SDL_KEYDOWN:
									if (event.key.keysym.sym == SDLK_RETURN) start = 1;
									break;
								}
							}
						}
						break;
					case 4:
						load(&player, &ally, &enemy, &worldTime, &score, bullets, &backgroundheight, &ibullets, screen, scrtex, window, renderer, charset);
						start = 0;
						while (!start)
						{
							while (SDL_PollEvent(&event)) {
								DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, czerwony, niebieski);
								sprintf_s(text, "Game loaded");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 221, text, charset);
								sprintf_s(text, "Click ENTER to resume game");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 237, text, charset);
								SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
								SDL_RenderClear(renderer);
								SDL_RenderCopy(renderer, scrtex, NULL, NULL);
								SDL_RenderPresent(renderer);
								switch (event.type) {
								case SDL_KEYDOWN:
									if (event.key.keysym.sym == SDLK_RETURN) start = 1;
									break;
								}
							}
						}
					}
					t1 = SDL_GetTicks();
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
				{
					if (ibullets < BULLETSONSCREEN - 1 && respawnTimebullet > 0.1)
					{
						respawnTimebullet = 0;
						createbullet(bullets, player, &ibullets);
					}
				}
				break;
			};
		};
	}

	free(bullets);

}
