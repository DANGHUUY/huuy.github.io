#include "PlayerPower.h"
PlayerPower::PlayerPower()
{
	number_ = 0;
}
PlayerPower::~PlayerPower()
{

}
void PlayerPower::AddPos(const int& xp)
{
	pos_list_.push_back(xp);
}
void PlayerPower::Init(SDL_Renderer* screen)
{
	LoadImage("IMG/player_power.png",screen);
	number_ = 3;
	if (pos_list_.size() > 0)
	{
		pos_list_.clear();

	}
	AddPos(20);
	AddPos(60);
	AddPos(100);
}
void PlayerPower::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list_.size(); i++)
	{
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}
void PlayerPower::Decrease()
{
	number_ --;
	pos_list_.pop_back();
} 
void PlayerPower::InitCrease()
{
	number_++;
	int last_pos = pos_list_.back();
	last_pos += 40;
	pos_list_.push_back(last_pos);

	Mix_Chunk* two_beep_chunk = Mix_LoadWAV(g_name_sound_increase);
	if (two_beep_chunk != NULL)
	{
		Mix_PlayChannel(-1, two_beep_chunk, 0);
	}
	else
	{
		std::cout << "error";
	}
		

}
PlayerMoney::PlayerMoney()
{
	x_pos_ = 0;
	y_pos_ = 0;
}
PlayerMoney::~PlayerMoney()
{

}
void PlayerMoney::Init(SDL_Renderer* screen)
{
	
	bool ret = LoadImage("IMG/money.png", screen);

}
void PlayerMoney::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}