// Allegro library
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

//Screen resolution
#define SCREEN_W 800
#define SCREEN_H 600
#define TILE_SIZE 32

// Game fps
#define FPS 60

// Direct
#define STAND 0
#define LEFT -1
#define RIGHT 1
#define UP -2
#define DOWN 2

// Game input keys
enum Keyboard { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE };
bool keyboard[5] = { false, false, false, false, false };

// Types of objects in the game
enum ObjectType { FOOD, BLOCK };
// Character Object Class
class CharacterObject
{
public:



	int SpriteWidth;
	int SpriteHeight;
	int NumbOfFrames;
	int PresentFrame;
	int F_W;
	int F_H;
	int frameDelay;
	int DelayCount;
	ALLEGRO_BITMAP *image;

	float angle;
	int Direct;
	float X;
	float Y;
	float S;

	// Class constructor
	CharacterObject()
	{
		X = 0;
		Y = 0;
		S = 0;
		Direct = 0;
		angle = 0;
		SpriteWidth = 0;
		SpriteHeight = 0;
		NumbOfFrames = 0;
		PresentFrame = 0;
		F_W = 0;
		F_H = 0;
		frameDelay = 0;
		DelayCount = 0;
		image = NULL;
	}


	void InitObject()
	{

		SpriteWidth = al_get_bitmap_width(image);
		SpriteHeight = al_get_bitmap_height(image);
		F_W = SpriteWidth / NumbOfFrames;
		F_H = SpriteHeight;
		al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
	}


	void DrawObj(ALLEGRO_DISPLAY *screen)
	{
		int PresentFrameX = PresentFrame * F_W;
		int PresentFrameY = 0;
		ALLEGRO_BITMAP *rotated = NULL;
		rotated = al_create_bitmap(F_W, F_H);
		al_set_target_bitmap(rotated);
		al_draw_bitmap_region(image, PresentFrameX, PresentFrameY, F_W, F_H, 0, 0, 0);
		al_set_target_bitmap(al_get_backbuffer(screen));


		/* Logic to handle the rotation of the sprite as per Direct
		0 degree is 0 radian
		90 degree is pi / 2
		180 degree is pi
		270 degree is 1.5 * pi
		360 degree is 2 * pi
		Check radian and degree conversion chart
		*/
		switch (Direct)
		{
		case STAND:
			angle = angle; // No rotation
			break;
		case RIGHT:
			angle = 0; // 0 degree rotation
			break;
		case LEFT:
			angle = ALLEGRO_PI; // 180 degree rotation
			break;
		case DOWN:
			angle = ALLEGRO_PI / 2; // 90 degree rotation
			break;
		case UP:
			angle = ALLEGRO_PI * 1.5; // 270 degree rotation
			break;

		}
		al_draw_rotated_bitmap(rotated, F_W / 2, F_H / 2, X, Y, angle, 0);
		al_destroy_bitmap(rotated);
		if (++DelayCount >= frameDelay)
		{
			if (++PresentFrame >= NumbOfFrames)PresentFrame = 0;

			if (Direct == STAND){ PresentFrame = 0; DelayCount = frameDelay; }
			DelayCount = 0;
		}
	}

	void UpdateLogic()
	{
		if (keyboard[KEY_UP])Direct = -2;
		if (keyboard[KEY_DOWN])Direct = 2;
		if (keyboard[KEY_LEFT])Direct = -1;
		if (keyboard[KEY_RIGHT])Direct = 1;


		switch (Direct)
		{
		case LEFT:
			X = X - S;
			break;
		case RIGHT:
			X = X + S;
			break;
		case UP:
			Y = Y - S;
			break;
		case DOWN:
			Y = Y + S; // Move along the y-axis
			break;
		case STAND: // No update in position
			break;
		}

		//Check for collision [Object moving out of the screen]
		if (X >= SCREEN_W)X = 10; // If Character Object position outside right of the screen
		if (Y >= SCREEN_H)Y = 10; // If Character Object position outside bottom of the screen
		if (X <= 0)X = SCREEN_W;  // If Character Object position outside the left of screen
		if (Y <= 0)Y = SCREEN_H;  // If Character Object position outsided the top of the
	}
};