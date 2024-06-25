#ifndef ini_h
#define ini_h

#define SCREEN_WIDTH	840
#define SCREEN_HEIGHT	480
#define MAP_START	-7680
#define MAP_END	8160
#define MAP_SIZE MAP_END * 2
#define CAR_START_WIDTH	(SCREEN_WIDTH/2) - 9 
#define CAR_HEIGHT	SCREEN_HEIGHT - 100
#define HITBOX_WIDTH	14
#define HITBOX_HEIGHT	30
#define HITBOXBULLET_WIDTH	6
#define HITBOXBULLET_HEIGHT	9
#define MAX_SPEED 500
#define _USE_MATH_DEFINES
#define MAX_OPTION 2
#define GAME_O 0

#define NOPOINTS_TIME 4
#define BULLETSONSCREEN 20
#define SPEEDBULLET 1000
#define RANGEBULLET 200

typedef struct {
	int x;
	int y;
	bool set;
	double speed;
	double distance_delta;
	int direction;
}car_t;

typedef struct {
	int x;
	int y;
	double distance_delta;
}bullet_t;

typedef struct {
	long long int score;
	double time;
}score_t;




#endif

