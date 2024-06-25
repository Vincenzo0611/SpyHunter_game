#include<SDL.h>
#include"structsanddefines.h"


int ini_screen(SDL_Surface** screen, SDL_Texture** scrtex, SDL_Window** window, SDL_Renderer** renderer);

void getColor(SDL_Surface* background, Uint8* red, Uint8* green, Uint8* blue, int x, int y);

void calTime(int* t1, int t2, int* frames, double* delta, double* worldTime, double* respawnTimeally, double* respawnTimebullet, double* fpsTimer, double* fps, long long int* score, double speed, int* noPoints, double* moveTime, bool count);
void calDistance(double* distance_d, int* distance, double delta, double speed, int* backgroundheight, car_t* player, car_t* ally, car_t* enemy, bullet_t* bullets, int ibullets);
bool outofRoad(car_t player, SDL_Surface* background);
bool onRoad(car_t player, SDL_Surface* background);
bool onSide(car_t player, SDL_Surface* background);
void list(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer);
