#include<SDL_image.h>
#include <SDL.h>
#include <iostream>
#include "CommonFunction.h"
#include"BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include"ThreatsObject.h"
#include "ExplosionObject.h"
#include"TextObject.h"
#include"sstream"
#include"PlayerPower.h"
#include "Geometric.h"
#include "BossObject.h"
#include <cstdlib>  // Để dùng rand() và srand()
#include <ctime>    // Để khởi tạo seed ngẫu nhiên

BaseObject g_background;
TTF_Font* font_time = NULL;

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret < 0)
	{
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("Defeat The Dark Monster", SDL_WINDOWPOS_UNDEFINED,
		                                                    SDL_WINDOWPOS_UNDEFINED,
															SCREEN_WIDTH,SCREEN_HEIGHT,
															SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		success = false;

	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
		// Khởi tạo SDL_mixer (âm thanh)
		
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			success = false;
		}

		g_sound_bullet[0] = Mix_LoadWAV(g_name_audio_bullet_main1);
		g_sound_bullet[1] = Mix_LoadWAV(g_name_audio_bullet_main2);
		g_sound_explosion = Mix_LoadWAV(g_name_audio_ex_main);
		g_sound_ex_main = Mix_LoadWAV(g_name_audio_ex_threats);
		g_sound_dragon = Mix_LoadWAV(g_name_audio_dragon);
		g_sound_flame = Mix_LoadWAV(g_name_audio_flame_boss);
		g_sound_chidori_effect = Mix_LoadWAV(g_name_audio_chidori_effect);
		if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_explosion == NULL|| g_sound_dragon==NULL
			|| g_sound_flame==NULL|| g_sound_chidori_effect==NULL)
		{
			return false;
		}
		if (TTF_Init()==-1)
		
		{
			success = false;
		}
		font_time = TTF_OpenFont("font/dlxfont_.ttf", 15);
		if (font_time == NULL)
		{
			success = false;
		}
	
	}

	return success;


}


bool LoadBackground()
{
	bool ret = g_background.LoadImage("IMG/backgroundha.jpeg", g_screen);

	if (ret == false)
		return false;
	return true;
}
void close()
{
	// Giải phóng nhạc nền
	Mix_HaltMusic();  // Dừng nhạc nền khi game kết thúc
	Mix_FreeMusic(g_music);
	g_music = NULL;
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();

}


std::vector<ThreatsObject*> MakeThreadList(MainObject*p_player)
{
	std::vector<ThreatsObject*> list_threats;


	ThreatsObject* dynamic_threats = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImage("IMG/threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_STATIC_THREAT);
			p_threat->set_x_pos(750 + i * 500);
			p_threat->set_y_pos(200);
			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen,p_player);

			p_threat->SetAnimationPos(pos1, pos2);

			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);
		}
	}

	

	ThreatsObject* threats_objs = new ThreatsObject[40];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImage("IMG/threat_level1.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(700 + i * 1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);
			
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen, p_player);
			list_threats.push_back(p_threat);
		}
	}
//	ThreatsObject* threats_objs = new ThreatsObject[20];
	
	
		ThreatsObject* p_threat = (threats_objs + 20);
		if (p_threat != NULL)
		{
			p_threat->LoadImage("IMG/threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(1216);
			p_threat->set_y_pos(384);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen, p_player);
			list_threats.push_back(p_threat);
		}
		ThreatsObject* p1_threat = (threats_objs + 21);
		if (p1_threat != NULL)
		{
			p1_threat->LoadImage("IMG/threat_level.png", g_screen);
			p1_threat->set_clips();
			p1_threat->set_x_pos(2752);
			p1_threat->set_y_pos(192);
			p1_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p1_threat->set_input_left(0);
			
			BulletObject* p1_bullet = new BulletObject();
			p1_threat->InitBullet(p1_bullet, g_screen, p_player);
			list_threats.push_back(p1_threat);
		}
		
		ThreatsObject* p2_threat = (threats_objs + 22);
		if (p2_threat != NULL)
		{
			p2_threat->LoadImage("IMG/threat_level.png", g_screen);
			p2_threat->set_clips();
			p2_threat->set_x_pos(2752);
			p2_threat->set_y_pos(192);
			p2_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p2_threat->set_input_left(0);

			BulletObject* p2_bullet = new BulletObject();
			p2_threat->InitBullet(p2_bullet, g_screen, p_player);
			list_threats.push_back(p2_threat);
		}
		ThreatsObject* p3_threat = (threats_objs + 23);
		if (p3_threat != NULL)
		{
			p3_threat->LoadImage("IMG/threat_level.png", g_screen);
			p3_threat->set_clips();
			p3_threat->set_x_pos(3904);
			p3_threat->set_y_pos(192);
			p3_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p3_threat->set_input_left(0);

			BulletObject* p3_bullet = new BulletObject();
			p3_threat->InitBullet(p3_bullet, g_screen, p_player);
			list_threats.push_back(p3_threat);
		}
		ThreatsObject* p4_threat = (threats_objs + 24);
		if (p4_threat != NULL)
		{
			p4_threat->LoadImage("IMG/threat_level.png", g_screen);
			p4_threat->set_clips();
			p4_threat->set_x_pos(6336);
			p4_threat->set_y_pos(128);
			p4_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p4_threat->set_input_left(0);

			BulletObject* p4_bullet = new BulletObject();
			p4_threat->InitBullet(p4_bullet, g_screen, p_player);
			list_threats.push_back(p4_threat);
		}
		ThreatsObject* p5_threat = (threats_objs + 25);
		if (p5_threat != NULL)
		{
			p5_threat->LoadImage("IMG/threat_level.png", g_screen);
			p5_threat->set_clips();
			p5_threat->set_x_pos(7680);
			p5_threat->set_y_pos(192);
			p5_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p5_threat->set_input_left(0);

			BulletObject* p5_bullet = new BulletObject();
			p5_threat->InitBullet(p5_bullet, g_screen, p_player);
			list_threats.push_back(p5_threat);
		}
		ThreatsObject* p6_threat = (threats_objs + 26);
		if (p6_threat != NULL)
		{
			p6_threat->LoadImage("IMG/threat_level.png", g_screen);
			p6_threat->set_clips();
			p6_threat->set_x_pos(9344);
			p6_threat->set_y_pos(256);
			p6_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p6_threat->set_input_left(0);

			BulletObject* p6_bullet = new BulletObject();
			p6_threat->InitBullet(p6_bullet, g_screen, p_player);
			list_threats.push_back(p6_threat);
		}
		ThreatsObject* p7_threat = (threats_objs + 27);
		if (p7_threat != NULL)
		{
			p7_threat->LoadImage("IMG/threat_level.png", g_screen);
			p7_threat->set_clips();
			p7_threat->set_x_pos(9920);
			p7_threat->set_y_pos(256);
			p7_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p7_threat->set_input_left(0);

			BulletObject* p7_bullet = new BulletObject();
			p7_threat->InitBullet(p7_bullet, g_screen, p_player);
			list_threats.push_back(p7_threat);
		}
		ThreatsObject* p8_threat = (threats_objs + 28);
		if (p8_threat != NULL)
		{
			p8_threat->LoadImage("IMG/threat_level.png", g_screen);
			p8_threat->set_clips();
			p8_threat->set_x_pos(10304);
			p8_threat->set_y_pos(190);
			p8_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p8_threat->set_input_left(0);

			BulletObject* p8_bullet = new BulletObject();
			p8_threat->InitBullet(p8_bullet, g_screen, p_player);
			list_threats.push_back(p8_threat);
		}
		ThreatsObject* p9_threat = (threats_objs + 29);
		if (p9_threat != NULL)
		{
			p9_threat->LoadImage("IMG/threat_level.png", g_screen);
			p9_threat->set_clips();
			p9_threat->set_x_pos(10880);
			p9_threat->set_y_pos(320);
			p9_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p9_threat->set_input_left(0);

			BulletObject* p9_bullet = new BulletObject();
			p9_threat->InitBullet(p9_bullet, g_screen, p_player);
			list_threats.push_back(p9_threat);
		}
		ThreatsObject* p10_threat = (threats_objs + 30);
		if (p10_threat != NULL)
		{
			p10_threat->LoadImage("IMG/threat_level.png", g_screen);
			p10_threat->set_clips();
			p10_threat->set_x_pos(11712);
			p10_threat->set_y_pos(320);
			p10_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p10_threat->set_input_left(0);

			BulletObject* p10_bullet = new BulletObject();
			p10_threat->InitBullet(p10_bullet, g_screen, p_player);
			list_threats.push_back(p10_threat);
		}
		ThreatsObject* p11_threat = (threats_objs + 31);
		if (p11_threat != NULL)
		{
			p11_threat->LoadImage("IMG/threat_level.png", g_screen);
			p11_threat->set_clips();
			p11_threat->set_x_pos(13248);
			p11_threat->set_y_pos(256);
			p11_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p11_threat->set_input_left(0);

			BulletObject* p11_bullet = new BulletObject();
			p11_threat->InitBullet(p11_bullet, g_screen, p_player);
			list_threats.push_back(p11_threat);
		}
		ThreatsObject* p12_threat = (threats_objs + 32);
		if (p12_threat != NULL)
		{
			p12_threat->LoadImage("IMG/threat_level.png", g_screen);
			p12_threat->set_clips();
			p12_threat->set_x_pos(15616);
			p12_threat->set_y_pos(448);
			p12_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p12_threat->set_input_left(0);
			
			BulletObject* p12_bullet = new BulletObject();
			p12_threat->InitBullet(p12_bullet, g_screen, p_player);
			list_threats.push_back(p12_threat);
		}
		ThreatsObject* p13_threat = (threats_objs + 33);
		if (p13_threat != NULL)
		{
			p13_threat->LoadImage("IMG/threat_level.png", g_screen);
			p13_threat->set_clips();
			p13_threat->set_x_pos(15872);
			p13_threat->set_y_pos(384);
			p13_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p13_threat->set_input_left(0);

			BulletObject* p13_bullet = new BulletObject();
			p13_threat->InitBullet(p13_bullet, g_screen, p_player);
			list_threats.push_back(p13_threat);
		}
		ThreatsObject* p14_threat = (threats_objs + 34);
		if (p14_threat != NULL)
		{
			p14_threat->LoadImage("IMG/threat_level.png", g_screen);
			p14_threat->set_clips();
			p14_threat->set_x_pos(16768);
			p14_threat->set_y_pos(320);
			p14_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p14_threat->set_input_left(0);

			BulletObject* p14_bullet = new BulletObject();
			p14_threat->InitBullet(p14_bullet, g_screen, p_player);
			list_threats.push_back(p14_threat);
		}
		ThreatsObject* p15_threat = (threats_objs + 35);
		if (p15_threat != NULL)
		{
			p15_threat->LoadImage("IMG/threat_level.png", g_screen);
			p15_threat->set_clips();
			p15_threat->set_x_pos(17088);
			p15_threat->set_y_pos(320);
			p15_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p15_threat->set_input_left(0);

			BulletObject* p15_bullet = new BulletObject();
			p15_threat->InitBullet(p15_bullet, g_screen, p_player);
			list_threats.push_back(p15_threat);
		}
		ThreatsObject* p16_threat = (threats_objs + 36);
		if (p16_threat != NULL)
		{
			p16_threat->LoadImage("IMG/threat_level.png", g_screen);
			p16_threat->set_clips();	
			p16_threat->set_x_pos(10304);
			p16_threat->set_y_pos(190);
			p16_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p16_threat->set_input_left(0);

			BulletObject* p16_bullet = new BulletObject();
			p16_threat->InitBullet(p16_bullet, g_screen, p_player);
			list_threats.push_back(p16_threat);
		}
	return list_threats;
}
bool restart=false;
bool ShowMenu() {
	bool quit = false;
	SDL_Event e;
	TextObject titleText,guideText[4];
	TTF_Font* font_time = TTF_OpenFont("font/dlxfont_.ttf", 20);
	if (font_time == NULL) {
		std::cerr << "Không thể tải font: " << TTF_GetError() << std::endl;
		return true;
	}
	g_music = Mix_LoadMUS("sound/invasion.mp3");  // Thay thế bằng đường dẫn tệp nhạc của bạn
	if (g_music == NULL) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		// Phát nhạc nền
		Mix_PlayMusic(g_music, -1);  // -1 có nghĩa là phát liên tục
	}
	titleText.SetColor(TextObject::WHITE_TEXT);  
	titleText.SetStr("GUIDE: A: Left, D: Right, W: Jump"); 
	titleText.LoadFromRenderText(font_time, g_screen);
	guideText[0].SetColor(TextObject::WHITE_TEXT);
	guideText[0].SetStr("LeftMouse: Attack, P: pause game");
	guideText[0].LoadFromRenderText(font_time, g_screen);
	guideText[1].SetColor(TextObject::WHITE_TEXT);
	guideText[1].SetStr("money/5 = 1 chidori : S + MouseLeft ");
	guideText[1].LoadFromRenderText(font_time, g_screen);
	guideText[2].SetColor(TextObject::WHITE_TEXT);
	guideText[2].SetStr("money/10 = 1 Heal : q ");
	guideText[2].LoadFromRenderText(font_time, g_screen);
	guideText[3].SetColor(TextObject::WHITE_TEXT);
	guideText[3].SetStr("on/off music : T ");
	guideText[3].LoadFromRenderText(font_time, g_screen);

	SDL_Texture* backgroundTexture = IMG_LoadTexture(g_screen, "IMG/menu.jpg");
	if (backgroundTexture == NULL) {
		std::cerr << "Không thể tải hình ảnh nền 'menu.jpg': " << SDL_GetError() << std::endl;
		return true;
	}

	
	SDL_Texture* playButtonTexture = IMG_LoadTexture(g_screen, "IMG/play_background.png");
	if (playButtonTexture == NULL) {
		std::cerr << "Không thể tải hình ảnh 'play_background.png': " << SDL_GetError() << std::endl;
	}

	
	SDL_Texture* exitButtonTexture = IMG_LoadTexture(g_screen, "IMG/exit_background.png");
	if (exitButtonTexture == NULL) {
		std::cerr << "Không thể tải hình ảnh 'exit_background.png': " << SDL_GetError() << std::endl;
	}

	
	SDL_Rect playButton = { 300, 200, 200, 50 }; // Nút Play
	SDL_Rect exitButton = { 300, 300, 200, 50 }; // Nút Exit

	while (!quit) {
		// Xử lý sự kiện
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;

				// Kiểm tra vị trí nhấn chuột cho nút Play
				if (x >= playButton.x && x <= playButton.x + playButton.w &&
					y >= playButton.y && y <= playButton.y + playButton.h) {
					std::cout << "Play Game" << std::endl;
					quit = true; // Thoát khỏi menu
					restart = true; // Đặt cờ để khởi động trò chơi
					return false; // Quay lại trò chơi
				}

				// Kiểm tra vị trí nhấn chuột cho nút Exit
				if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
					y >= exitButton.y && y <= exitButton.y + exitButton.h) {
					std::cout << "Exit Game" << std::endl;
					quit = true; // Thoát khỏi vòng lặp
					return true; // Kết thúc chương trình
				}
			}
		}
		
		// Vẽ nền menu
		SDL_RenderCopy(g_screen, backgroundTexture, NULL, NULL);
		titleText.RenderText(g_screen, 200, 100);
		guideText[0].RenderText(g_screen, 250, 150);
		guideText[1].RenderText(g_screen, 550, 200);
		guideText[2].RenderText(g_screen, 550, 250);
		guideText[3].RenderText(g_screen, 550, 300);
		// Vẽ nút Play
		if (playButtonTexture) {
			SDL_RenderCopy(g_screen, playButtonTexture, NULL, &playButton);
		}

		// Vẽ nút Exit
		if (exitButtonTexture) {
			SDL_RenderCopy(g_screen, exitButtonTexture, NULL, &exitButton);
		}

		// Cập nhật màn hình
		SDL_RenderPresent(g_screen);
	}

	// Giải phóng tài nguyên
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(playButtonTexture);
	SDL_DestroyTexture(exitButtonTexture);

	return false; // Nếu không chọn Exit, quay lại trò chơi
}

bool isPaused = false;  // Biến để kiểm tra trạng thái Pause

void RestartGame(GameMap& game_map, MainObject& p_player, std::vector<ThreatsObject*>& threats_list, PlayerPower& player_power, PlayerMoney& player_money, BossObject& bossObject, int& num_die, Uint32& val_time, UINT& mark_value) {
	// Khôi phục lại các biến trạng thái
	num_die = 0;
	mark_value = 0;
	val_time = 300; // Thời gian khởi đầu
	std::cout << "Restarting game. val_time set to: " << val_time << std::endl; // Log để kiểm tra
	// Khởi tạo lại bản đồ

	 // Dừng nhạc nền hiện tại
	Mix_HaltMusic();

	// Giải phóng nhạc nền cũ nếu có
	if (g_music != NULL) {
		Mix_FreeMusic(g_music);
		g_music = NULL;
	}
	std::srand(std::time(0));

	
	int random_choice = std::rand() % 2; 

	const char* music_path = (random_choice == 0) ? "sound/kokuten_10pt.mp3" : "sound/yugioh_low.mp3";
	g_music = Mix_LoadMUS(music_path);  
	if (g_music == NULL) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		
		Mix_PlayMusic(g_music, -1);  
	}
	/*/ Tải nhạc nền mới
	g_music = Mix_LoadMUS("sound/kokuten.mp3");  // Thay thế bằng đường dẫn tệp nhạc của bạn
	if (g_music == NULL) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		// Phát nhạc nền
		Mix_PlayMusic(g_music, -1);  // -1 có nghĩa là phát liên tục
	}*/
	game_map.LoadMap("mapgamer.txt");
	game_map.LoadTiles(g_screen);

	// Khởi tạo lại đối tượng người chơi
	p_player.SetRect(0, 0); // Đặt lại vị trí người chơi
	p_player.LoadImage("IMG/player_right.png", g_screen);
	p_player.set_clips();
	p_player.WALK_NONE;
	p_player.Reset();

	// Khởi tạo lại sức mạnh của người chơi
	player_power.Init(g_screen);

	// Khởi tạo lại tiền của người chơi
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

	// Xóa danh sách kẻ thù cũ
	for (auto& threat : threats_list) {
		if (threat) {
			threat->Free();
			
			threat = nullptr;
		}
	}
	threats_list.clear();

	// Tạo lại danh sách kẻ thù
	threats_list = MakeThreadList(&p_player);
	bossObject.Reset();
	// Khởi tạo lại boss
	bossObject.LoadImg("IMG/boss_object.png", g_screen);
	bossObject.set_clips();
	int xPosBoss = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH * 0.6;
	bossObject.set_xpos(xPosBoss);
	bossObject.set_ypos(10);
	bossObject.SetHP(150);
	
}

bool PauseMenu() {
	bool quit = false;
	bool backToMenu = false;
	SDL_Event e;

	// Tải các hình ảnh nút "Continue" và "Back to Menu"
	SDL_Texture* continueButtonTexture = IMG_LoadTexture(g_screen, "IMG/continue_button.png");
	if (continueButtonTexture == NULL) {
		printf("Không thể tải hình ảnh 'continue_button.png' %s\n", SDL_GetError());
	}

	SDL_Texture* menuButtonTexture = IMG_LoadTexture(g_screen, "IMG/back_to_menu.png");
	if (menuButtonTexture == NULL) {
		printf("Không thể tải hình ảnh 'back_to_menu.png' %s\n", SDL_GetError());
	}

	SDL_Rect continueButton = { 540, 200, 200, 50 };
	SDL_Rect menuButton = { 540, 300, 200, 50 };

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				isPaused = false; // Thoát hoàn toàn
				return false;     // Thoát game
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;
				
				// Kiểm tra nhấn nút "Continue"
				if (x >= continueButton.x && x <= continueButton.x + continueButton.w &&
					y >= continueButton.y && y <= continueButton.y + continueButton.h) {
					quit = true; // Quay lại trò chơi
					
					isPaused = false;
					return false; // Không trở về menu
				}

				// Kiểm tra nhấn nút "Back to Menu"
				if (x >= menuButton.x && x <= menuButton.x + menuButton.w &&
					y >= menuButton.y && y <= menuButton.y + menuButton.h) {
					quit = true;   // Thoát khỏi menu Pause
					backToMenu = true; // Trở về menu chính
				}
			}
		}

		// Vẽ nền menu Pause
		//SDL_RenderClear(g_screen);

		// Vẽ nút "Continue"
		SDL_RenderCopy(g_screen, continueButtonTexture, NULL, &continueButton);

		// Vẽ nút "Back to Menu"
		SDL_RenderCopy(g_screen, menuButtonTexture, NULL, &menuButton);

		SDL_RenderPresent(g_screen);
	}

	// Giải phóng texture
	SDL_DestroyTexture(continueButtonTexture);
	SDL_DestroyTexture(menuButtonTexture);

	return backToMenu; // Trả về true nếu chọn "Back to Menu"
}






void ShowPauseMenu() {
	bool quit = false;
	SDL_Event e;

	// Tạo các đối tượng TextObject
	TextObject continueText, menuText;
	continueText.SetColor(TextObject::RED_TEXT);
	continueText.SetStr("Continue");
	continueText.LoadFromRenderText(font_time, g_screen);

	menuText.SetColor(TextObject::RED_TEXT);
	menuText.SetStr("Back to Menu");
	menuText.LoadFromRenderText(font_time, g_screen);

	// Tọa độ và kích thước nút
	SDL_Rect continueButton = { 540, 200, 200, 50 };
	SDL_Rect menuButton = { 540, 300, 200, 50 };

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				isPaused = false;  // Thoát hoàn toàn
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;
				// Kiểm tra nhấn nút "Continue"
				if (x >= continueButton.x && x <= continueButton.x + continueButton.w && y >= continueButton.y && y <= continueButton.y + continueButton.h) {
					quit = true;  // Quay lại trò chơi
					if (!LoadBackground()) {  // Kiểm tra nếu tải nền không thành công
						std::cerr << "Failed to load background!" << std::endl;
					}
					isPaused = false;
				}
				// Kiểm tra nhấn nút "Back to Menu"
				if (x >= menuButton.x && x <= menuButton.x + menuButton.w && y >= menuButton.y && y <= menuButton.y + menuButton.h) {
					quit = true;
					
					ShowMenu();  // Quay lại menu chính
					isPaused = false;
				}
			}
		}

		// Vẽ nền menu Pause
		SDL_RenderClear(g_screen);

		// Vẽ các nút "Continue" và "Back to Menu"
		continueText.RenderText(g_screen, continueButton.x, continueButton.y);
		menuText.RenderText(g_screen, menuButton.x, menuButton.y);

		SDL_RenderPresent(g_screen);
	}
}

bool ShowYouLoseMenu() {
	bool quit = false;
	SDL_Event e;
	TextObject titleText;

	titleText.SetColor(TextObject::WHITE_TEXT);  // White color
	titleText.SetStr("You Lose!");  // Title message
	titleText.LoadFromRenderText(font_time, g_screen);
	g_music = Mix_LoadMUS("sound/SoulSociety.mp3");  // Thay thế bằng đường dẫn tệp nhạc của bạn
	if (g_music == NULL) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		// Phát nhạc nền
		Mix_PlayMusic(g_music, -1);  // -1 có nghĩa là phát liên tục
	}
	SDL_Texture* LoseBackGround = IMG_LoadTexture(g_screen, "IMG/lose.jpeg");
	if (LoseBackGround == NULL) {
		std::cerr << "Cannot load image 'replay_button.png': " << SDL_GetError() << std::endl;
	}

	SDL_Texture* replayButtonTexture = IMG_LoadTexture(g_screen, "IMG/replay_button.png");
	if (replayButtonTexture == NULL) {
		std::cerr << "Cannot load image 'replay_button.png': " << SDL_GetError() << std::endl;
	}

	SDL_Texture* exitButtonTexture = IMG_LoadTexture(g_screen, "IMG/exit_background.png");
	if (exitButtonTexture == NULL) {
		std::cerr << "Cannot load image 'exit_button.png': " << SDL_GetError() << std::endl;
	}

	SDL_Rect replayButton = { 350, 500, 200, 50 }; // Replay button
	SDL_Rect exitButton = { 700, 500, 200, 50 }; // Exit button

	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;
				if (x >= replayButton.x && x <= replayButton.x + replayButton.w &&
					y >= replayButton.y && y <= replayButton.y + replayButton.h) {
					std::cout << "Replay Game" << std::endl;
					restart = true;
					quit = true; // Exit menu
					return false; // Return to replay the game
				}
				if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
					y >= exitButton.y && y <= exitButton.y + exitButton.h) {
					std::cout << "Exit Game" << std::endl;
					quit = true; // Exit the loop
					return true; // End the program
				}
			}
		}
		titleText.RenderText(g_screen, 200, 100);
	     SDL_RenderCopy(g_screen, LoseBackGround, NULL, NULL);
		if (replayButtonTexture) {
			SDL_RenderCopy(g_screen, replayButtonTexture, NULL, &replayButton);
		}	
		if (exitButtonTexture) {
			SDL_RenderCopy(g_screen, exitButtonTexture, NULL, &exitButton);
		}
        SDL_RenderPresent(g_screen);
	}
    SDL_DestroyTexture(LoseBackGround);
	SDL_DestroyTexture(replayButtonTexture);
	SDL_DestroyTexture(exitButtonTexture);

	return false; 
}
bool ShowVictory(int remainingTime, int coinsCollected, int marksDefeated) {
	bool quit = false;
	SDL_Event e;
	TextObject victoryText;
	TextObject timeText;
	TextObject coinsText;
	TextObject marksText;

	// Set up text color
	victoryText.SetColor(TextObject::WHITE_TEXT);
	timeText.SetColor(TextObject::WHITE_TEXT);
	coinsText.SetColor(TextObject::WHITE_TEXT);
	marksText.SetColor(TextObject::WHITE_TEXT);

	// Set up victory message
	victoryText.SetStr("Congratulations! You Win!");
	victoryText.LoadFromRenderText(font_time, g_screen);

	// Set up remaining time message
	std::string timeMessage = "Time Remaining: " + std::to_string(remainingTime) + " seconds";
	timeText.SetStr(timeMessage);
	timeText.LoadFromRenderText(font_time, g_screen);

	// Set up coins collected message
	std::string coinsMessage = "Coins Collected: " + std::to_string(coinsCollected);
	coinsText.SetStr(coinsMessage);
	coinsText.LoadFromRenderText(font_time, g_screen);

	// Set up marks defeated message
	std::string marksMessage = "Marks Defeated: " + std::to_string(marksDefeated);
	marksText.SetStr(marksMessage);
	marksText.LoadFromRenderText(font_time, g_screen);
	g_music = Mix_LoadMUS("sound/treachery.mp3");  // Thay thế bằng đường dẫn tệp nhạc của bạn
	if (g_music == NULL) {
		std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		// Phát nhạc nền
		Mix_PlayMusic(g_music, -1);  // -1 có nghĩa là phát liên tục
	}
	// Load button textures
	SDL_Texture* replayButtonTexture = IMG_LoadTexture(g_screen, "IMG/replay_button.png");
	SDL_Texture* exitButtonTexture = IMG_LoadTexture(g_screen, "IMG/exit_background.png");

	// Define button rectangles
	SDL_Rect replayButton = { 350, 500, 200, 50 }; // Replay button
	SDL_Rect exitButton = { 700, 500, 200, 50 }; // Exit button

	// Main loop for the victory screen
	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;

				// Check if the replay button is clicked
				if (x >= replayButton.x && x <= replayButton.x + replayButton.w &&
					y >= replayButton.y && y <= replayButton.y + replayButton.h) {
					std::cout << "Replay Game" << std::endl;
					restart = true;
					quit = true; // Exit victory screen
					return false; // Return to replay the game
				}

				// Check if the exit button is clicked
				if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
					y >= exitButton.y && y <= exitButton.y + exitButton.h) {
					std::cout << "Exit Game" << std::endl;
					quit = true; // Exit the loop
					return true; // End the program
				}
			}
		}

		// Clear the screen
		SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255); // Set background color (black)
		SDL_RenderClear(g_screen);

		// Render the victory text
		victoryText.RenderText(g_screen, 200, 100); // Adjust position as needed

		// Render the remaining time text
		timeText.RenderText(g_screen, 200, 200); // Adjust position as needed

		// Render the coins collected text
		coinsText.RenderText(g_screen, 200, 300); // Adjust position as needed

		// Render the marks defeated text
		marksText.RenderText(g_screen, 200, 400); // Adjust position as needed

		// Draw the replay button
		if (replayButtonTexture) {
			SDL_RenderCopy(g_screen, replayButtonTexture, NULL, &replayButton);
		}

		// Draw the exit button
		if (exitButtonTexture) {
			SDL_RenderCopy(g_screen, exitButtonTexture, NULL, &exitButton);
		}

		// Update the screen
		SDL_RenderPresent(g_screen);
	}

	// Free resources
	SDL_DestroyTexture(replayButtonTexture);
	SDL_DestroyTexture(exitButtonTexture);
}
int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
	

	if (InitData() == false)
	{
		std::cerr << "Initialization failed!" << std::endl;
		return -1;
	}
	
	if (LoadBackground() == false) {
		std::cerr << "Loading background failed!" << std::endl;
		return -1;
	}
	
	GameMap game_map;
	game_map.LoadMap("mapgamer.txt");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
    p_player.LoadImage("IMG/player_right.png",g_screen);
	p_player.set_clips();

	PlayerPower player_power;
	player_power.Init(g_screen);

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

	std::vector<ThreatsObject*>threats_list = MakeThreadList(&p_player);
	 //BOSS
	BossObject bossObject;
	bossObject.LoadImg("IMG/boss_object.png", g_screen);
	bossObject.set_clips();
	int xPosBoss = MAX_MAP_X * TILE_SIZE -SCREEN_WIDTH * 0.6;
	bossObject.set_xpos(xPosBoss);
	bossObject.set_ypos(10);

	bossObject.SetHP(150);
	int typeBullet = 100;

	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImage("IMG/explosion.png", g_screen);
	if (!tRet) return -1;
	exp_threat.set_clip();

	ExplosionObject exp_main;
	bool mRet = exp_main.LoadImage("IMG/explosion.png", g_screen);
	exp_main.set_clip();
	if (!mRet) return -1;
	

	int num_die = 0;
	bool status_voice = true;
	// text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;

	Uint32 val_time=300;
	int comeback = 0;
	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);

	bool is_quit = false;
	bool quitMenu = ShowMenu();
	if (quitMenu == true) {
		is_quit = true;
	}
	Uint32 start_time = SDL_GetTicks() / 1000;
	while (!is_quit)
	{   
		if (restart == true)
		{
             RestartGame(game_map, p_player, threats_list, player_power, player_money, bossObject, num_die, val_time, mark_value);
			 start_time = SDL_GetTicks() / 1000;
			 restart = false;
		}	
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			// Nếu nhấn phím P, thay đổi trạng thái pause
			if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_p)
			{
				isPaused = !isPaused;  // Đảo ngược trạng thái pause
			}
			p_player.MandelInputAction(g_event, g_screen);
		}
		if (isPaused) {
			bool backToMenu = PauseMenu(); // Trả về true nếu chọn "Back to Menu"
			if (backToMenu) {
				bool quitMenu = ShowMenu(); // Hiển thị Main Menu
				if (quitMenu) {
					is_quit = true; // Thoát nếu chọn "Exit" từ Main Menu
					break;
				}
				isPaused = false; // Quay lại trò chơi nếu chọn "Play" từ Main Menu
			}
			else if (is_quit) {
				// Nếu chọn Exit từ Pause Menu
				break;
			}
		}
		if (!isPaused)
		{

			//LoadBackground();
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			SDL_RenderClear(g_screen);

			g_background.Render(g_screen, NULL);

			Map map_data = game_map.getMap();

			p_player.HandleBullet(g_screen);
			p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
			p_player.DoPlayer(map_data, player_power, num_die);
			p_player.Show(g_screen);
			p_player.UpdateHP(player_power, num_die);
			p_player.set_status_voice(status_voice);
			
				if (status_voice == false)
				{
					Mix_HaltMusic();
			    }
				else
				{
					if (!Mix_PlayingMusic() && g_music != NULL) // Chỉ bật nhạc nếu nhạc chưa được phát
					{
						Mix_PlayMusic(g_music, -1); // Phát lại nhạc
					}
				}
				
			
			if (num_die == 3)
			{
				std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
				bool exitgame = ShowYouLoseMenu();
				if (exitgame == true)
				{

					close();
					SDL_Quit();
					return 0;
				}
			}
			
			game_map.SetMap(map_data);
			bossObject.RenderHP(g_screen, font_time);
			game_map.DrawMap(g_screen);

			// Draw Geometric
			GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
			ColorData color_data(0, 80, 150);
			Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

			GeometricFormat outlineSize(1, 1, SCREEN_WIDTH - 1, 38);
			ColorData color_data2(255, 255, 255);
			Geometric::RenderOutline(outlineSize, color_data2, g_screen);


			player_power.Show(g_screen);
			
			player_money.Show(g_screen);
			
			
			for (int i = 0; i < threats_list.size(); i++)
			{

				ThreatsObject* p_threat = threats_list.at(i);
				if (p_threat != NULL)
				{
					p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
					p_threat->ImpMoveType(g_screen);
					p_threat->DoPlayer(map_data);
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
					p_threat->Show(g_screen);


					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					p_threat->UpdateBullets(&p_player);
					
					std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
					for (int jj = 0; jj < tBullet_list.size(); ++jj)
					{
						BulletObject* pt_bullet = tBullet_list.at(jj);
						if (pt_bullet)
						{
							bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
							if (bCol1)
							{
								p_threat->RemoveBullet(jj);
								break;
							}
						}
					}

					SDL_Rect rect_threat = p_threat->GetRectFrame();
					bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
					if (bCol1 || bCol2)
					{
						int	width_exp_frame = exp_main.get_frame_width();
						int	heigh_exp_frame = exp_main.get_frame_heigh();
						for (int ex = 0; ex < 4; ex++)
						{
							int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
							int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - heigh_exp_frame * 0.5;

							exp_main.set_frame(ex);
							exp_main.SetRect(x_pos, y_pos);
							exp_main.Show(g_screen);
							SDL_RenderPresent(g_screen);
						}
						Mix_PlayChannel(-1, g_sound_ex_main, 0);
						num_die++;
						if (num_die <= 3)
						{
							p_player.SetRect(0, 0);
							p_player.set_comeback_time(60);
							SDL_Delay(1000);
							player_power.Decrease();
							player_power.Render(g_screen);
							continue;

						}
						else
						{
							std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
							bool exitgame = ShowYouLoseMenu();
								if (exitgame == true)
							{
									p_threat->Free();
									close();
									SDL_Quit();
									return 0;
							}
								else
								{
									RestartGame(game_map, p_player, threats_list, player_power, player_money, bossObject, num_die, val_time, mark_value);
									start_time = SDL_GetTicks() / 1000;
								}
						}
					}


				}
			}

				
			int frame_exp_width = exp_threat.get_frame_width();
			int frame_exp_heigh = exp_threat.get_frame_heigh();

			std::vector<BulletObject*>bullet_arr = p_player.get_bullet_list();
			for (int r = 0; r < bullet_arr.size(); ++r)
			{
				BulletObject* p_bullet = bullet_arr.at(r);
				if (p_bullet != NULL)
				{
					for (int t = 0; t < threats_list.size(); ++t)
					{
						ThreatsObject* obj_threat = threats_list.at(t);
						if (obj_threat != NULL)
						{

							SDL_Rect tRect;
							tRect.x = obj_threat->GetRect().x;
							tRect.y = obj_threat->GetRect().y;
							tRect.w = obj_threat->get_width_frame();
							tRect.h = obj_threat->get_height_frame();

							SDL_Rect bRect = p_bullet->GetRect();

							bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
							p_player.set_type_bullet(typeBullet);
							if (bCol)

							{
								mark_value++;	
								for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
								{
									int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
									int y_pos = p_bullet->GetRect().y - frame_exp_heigh * 0.5;

									exp_threat.set_frame(ex);
									exp_threat.SetRect(x_pos, y_pos);
									exp_threat.Show(g_screen);
								}
								if (typeBullet == MainObject::CHIDORI)
								{
									mark_value = mark_value + 5;
									
									obj_threat->Free();
									threats_list.erase(threats_list.begin() + t);
									Mix_PlayChannel(-1, g_sound_chidori_effect, 0);

								}
								else
								{
									p_player.RemoveBullet(r);
									obj_threat->Free();
									threats_list.erase(threats_list.begin() + t);
									Mix_PlayChannel(-1, g_sound_explosion, 0);
								}

								

								

							}
						}
					}
				}
			}

			//SHOW GAME TIME
			
			std::string str_time = "TIME ";
			Uint32 current_time = SDL_GetTicks() / 1000; // Lấy thời gian hiện tại
			val_time = 300 - (current_time - start_time); 
			if (val_time <= 0)
			{
				std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
				bool exitgame = ShowYouLoseMenu();
				if (exitgame == true)
				{
					
					close();
					SDL_Quit();
					return 0;
				}
				else
				{
					RestartGame(game_map, p_player, threats_list, player_power, player_money, bossObject, num_die, val_time, mark_value);
					start_time = SDL_GetTicks() / 1000;
				}
			}
			else
			{
				std::string str_val = std::to_string(val_time);
				str_time += str_val;
				time_game.SetText(str_time);
				time_game.LoadFromRenderText(font_time, g_screen);
				time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
			}
			
			std::string val_str_mark = std::to_string(mark_value);
			std::string strMark("Mark: ");
			strMark += val_str_mark;

			mark_game.SetText(strMark);
			mark_game.LoadFromRenderText(font_time, g_screen);
			mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

			int money_count = p_player.GetMoneyCount();
			std::string money_str = std::to_string(money_count);



			money_game.SetText(money_str);
			money_game.LoadFromRenderText(font_time, g_screen);
			money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

			// Show Boss
			int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x) - 2960;
			static bool boss_turn_up = false;
			
			bossObject.UpdateBullets(&p_player);
			if (val <= SCREEN_WIDTH)
			{
				bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
				bossObject.DoPlayer(map_data);
				bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				bossObject.Show(g_screen);
				if (!boss_turn_up)
				{
					
							Mix_HaltMusic();
							const char* music_path1 = "sound/shingeki no kyojin.mp3";
							g_music = Mix_LoadMUS(music_path1);
							if (g_music == NULL) {
								std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
							}
							else 
							{

								Mix_PlayMusic(g_music, -1);
							}
						
					
					Mix_PlayChannel(-1, g_sound_dragon, 0);
					boss_turn_up = true;
					std::cout << boss_turn_up;
				}
				

				SDL_Rect rectb_player = p_player.GetRectFrame();
				bool bCol3 = false;

				// Duyệt qua các đạn của boss
				std::vector<BulletObject*> bBullet_list = bossObject.get_bullet_list();
				
					std::vector<int> bullets_to_remove;  // Đánh dấu các đạn cần xóa
					for (int bb = 0; bb < bBullet_list.size(); ++bb)
					{
						BulletObject* pb_bullet = bBullet_list.at(bb);
						if (pb_bullet)
						{
							bCol3 = SDLCommonFunc::CheckCollision(pb_bullet->GetRect(), rectb_player);
							if (bCol3)
							{
								bullets_to_remove.push_back(bb);
								//bossObject.RemoveBullet(bb);
								num_die++;
								
								int width_exp_frame = exp_main.get_frame_width();
								int heigh_exp_frame = exp_main.get_frame_heigh();
								for (int ex = 0; ex < 4; ex++)
								{
									int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
									int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - heigh_exp_frame * 0.5;

									exp_main.set_frame(ex);
									exp_main.SetRect(x_pos, y_pos);
									exp_main.Show(g_screen);
									SDL_RenderPresent(g_screen);
								}
								Mix_PlayChannel(-1, g_sound_ex_main, 0);
								if (num_die <= 3)//them
								{

									p_player.SetRect(0, 0);
									p_player.set_comeback_time(60);
									SDL_Delay(1000);
									player_power.Decrease();
									player_power.Render(g_screen);
									continue;

								}
								else
								{
									std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
									std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
									bool exitgame = ShowYouLoseMenu();
									if (exitgame == true)
									{

										bossObject.Free();
										close();
										SDL_Quit();
										return 0;
									}
									else
									{
										RestartGame(game_map, p_player, threats_list, player_power, player_money, bossObject, num_die, val_time, mark_value);
										start_time = SDL_GetTicks() / 1000;
										boss_turn_up = false;
									}
								}
							}
						}
					}

					// Xóa các đạn đã đánh dấu
					for (int i = bullets_to_remove.size() - 1; i >= 0; --i)
					{
						bossObject.RemoveBullet(bullets_to_remove[i]);
					}
				
				// Duyệt qua các đạn của người chơi
				std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
				bool bCol4 = false;  // Ensure that bCol4 is declared before usage
				std::vector<int> player_bullets_to_remove;
				for (int r = 0; r < bullet_arr.size(); ++r)
				{
					BulletObject* pc_bullet = bullet_arr.at(r);
					if (pc_bullet != NULL)
					{
						SDL_Rect tRect;
						tRect.x = bossObject.GetRect().x;
						tRect.y = bossObject.GetRect().y;
						tRect.w = bossObject.get_width_frame();
						tRect.h = bossObject.get_height_frame();

						SDL_Rect bRect = pc_bullet->GetRect();
						bCol4 = SDLCommonFunc::CheckCollision(bRect, tRect); 
						p_player.set_type_bullet(typeBullet);
						if (bCol4)
						{
							if (typeBullet == MainObject::NORMAL)
							{
								mark_value++;
								bossObject.TakeDamage(1);
								bossObject.RenderHP(g_screen, font_time);
								std::cout << "Boss hit! HP: " << bossObject.GetCurrentHP() << std::endl;
								
							}
							else
							{
								mark_value = mark_value + 10;
								bossObject.TakeDamage(3);
								bossObject.RenderHP(g_screen, font_time);
								std::cout << "Boss hit! HP: " << bossObject.GetCurrentHP() << std::endl;
								Mix_PlayChannel(-1, g_sound_chidori_effect, 0);
							}
							
							
							player_bullets_to_remove.push_back(r);
							
							break; 
						}
					}
				}
				for (int i = player_bullets_to_remove.size() - 1; i >= 0; --i)
				{
					p_player.RemoveBullet(player_bullets_to_remove[i]);
				}
				// Nếu có va chạm
				if (bCol3 || bCol4)
				{
					int width_exp_frame = exp_main.get_frame_width();
					int heigh_exp_frame = exp_main.get_frame_heigh();
					for (int ex = 0; ex < 4; ex++)
					{
						int x_pos = (bossObject.GetRect().x + bossObject.get_width_frame() * 0.5) - width_exp_frame * 0.5;
						int y_pos = (bossObject.GetRect().y + bossObject.get_height_frame() * 0.5) - heigh_exp_frame * 0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}

					if (bossObject.GetCurrentHP() == 145|| bossObject.GetCurrentHP() ==80||bossObject.GetCurrentHP()==50)
					{
						
				     Mix_PlayChannel(-1, g_sound_dragon, 0);
				    }
					
					 if (bossObject.GetCurrentHP() == 0)
					{
						std::cout << "Collision detected at frame " << fps_timer.get_ticks() << std::endl;
						bossObject.Free();
						/*if (MessageBox(NULL, L" Chuc Mung ", L"BAN DA CHIEN THANG", MB_OK | MB_ICONSTOP) == IDOK)
						{
							
							close();
							SDL_Quit();
							return 0;
						}*/
						if (ShowVictory(val_time, money_count, mark_value) == true)
						{
							close();
							SDL_Quit();
							return 0;
						}
						else
						{
							RestartGame(game_map, p_player, threats_list, player_power, player_money, bossObject, num_die, val_time, mark_value);
							start_time = SDL_GetTicks() / 1000;
							boss_turn_up = true;
						}
					}
				}
			}
		}
        else
         {
	     // Nếu trò chơi đang tạm dừng, hiển thị menu pause
	     ShowPauseMenu();
	     }

		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;//ms

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) {
				SDL_Delay(delay_time);
			}
		}

	}

	for (int i = 0; i < threats_list.size(); i++)
	{
		ThreatsObject* p_threat = threats_list.at(i);
		if (p_threat)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}
	threats_list.clear();

	close();
	return 0;
}	