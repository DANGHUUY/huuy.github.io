#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include<windows.h>
#include<string.h>
#include <vector>
#include<SDL.h>
#include<iostream>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include"sstream"


static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Music* g_music = NULL;
static Mix_Chunk* g_sound_bullet[3];
static Mix_Chunk* g_sound_explosion = NULL;
static Mix_Chunk* g_sound_ex_main = NULL;
static Mix_Chunk* g_sound_dragon = NULL;
static Mix_Chunk* g_sound_flame = NULL;
static Mix_Chunk* g_sound_chidori = NULL;
static Mix_Chunk* g_sound_chidori_effect = NULL;
static Mix_Chunk* g_sound_rasengan_effect = NULL;

static char g_name_audio_bullet_main1[] = { "sound//Fire.wav" };
static char g_name_audio_bullet_main2[] = { "sound//Laser.wav" };
static char g_name_audio_ex_main[] = { "sound//Explosion+1.wav" };
static char g_name_audio_ex_threats[] = { "sound//Bomb1.wav" };
static char g_name_sound_increase[] = { "sound//two_beep.mp3" };
static char g_name_audio_dragon[] = { "sound//colossal_dragon.mp3" };
static char g_name_audio_flame_boss[] = { "sound//flame.mp3" };
static char g_name_audio_chidori[] = { "sound//chidori.mp3" };
static char g_name_audio_chidori_effect[] = { "sound//chidori_effect.mp3" };
static char g_name_audio_rasengan_effect[] = { "sound//rasengan.mp3" };
static char kSoundBeep[] = { "sound//beep_.wav" };
// screen
const int FRAME_PER_SECOND = 25; //FPS so frame /1s
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;



#define BLANK_TILE 0
#define TILE_SIZE 64
#define	MAX_MAP_X 400
#define MAX_MAP_Y 10
#define STATE_MONEY 4// COIN
#define STATE_COLLECTED_MONEY  0
typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;

}Input;



typedef struct Map
{
    int start_x_;
    int start_y_;
    int max_x_;
    int max_y_;
    int tile[MAX_MAP_Y][MAX_MAP_X];
    std::string file_name_;

    // Constructor khởi tạo các giá trị mặc định
    Map()
        : start_x_(0), start_y_(0), max_x_(0), max_y_(0), file_name_("")
    {
        // Khởi tạo mảng tile (nếu cần)
        for (int i = 0; i < MAX_MAP_Y; ++i) {
            for (int j = 0; j < MAX_MAP_X; ++j) {
                tile[i][j] = 0;  // Hoặc giá trị mặc định khác nếu cần
            }
        }
    }
} Map;


namespace SDLCommonFunc
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    
}
#endif