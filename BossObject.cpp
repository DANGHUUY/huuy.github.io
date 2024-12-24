

#include "BossObject.h"
#include <SDL_ttf.h> 

BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    think_time_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    has_attacked_stage_140 = false;
    has_attacked_stage_70 = false;
    has_attacked_stage_40 = false;
    
    max_hp_ = 150;  // HP tối đa
    current_hp_ = 150;  // HP hiện tại
}

BossObject::~BossObject()
{

}

bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImage(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / FRAME_NUM_32;
        height_frame_ = rect_.h;
    }

    return ret;
}

void BossObject::set_clips()
{
    //Clip the sprites
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_32; ++i)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}


void BossObject::Show(SDL_Renderer* des)
{
    if (think_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ >= 32)
        {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
        if (currentClip != NULL)
        {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
        }
        

        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
    }
}


void BossObject::RenderHP(SDL_Renderer* screen, TTF_Font* font) {
    // Vị trí thanh HP (so với màn hình)
    int x_pos = x_pos_ - map_x_ + 20;  // map_x_ là tọa độ bản đồ
    int y_pos = y_pos_ - map_y_ - 20;  // Điều chỉnh để đặt gần Boss

    // Kích thước thanh HP
    int bar_width = 100;
    int bar_height = 10;

    // Tính toán chiều rộng thanh HP theo tỷ lệ
    int hp_width = (current_hp_ * bar_width) / max_hp_;

    // Vẽ thanh HP nền (màu đỏ)
    SDL_Rect background_bar = { x_pos, y_pos, bar_width, bar_height };
    SDL_SetRenderDrawColor(screen, 255, 0, 0, 255);  // Màu đỏ
    SDL_RenderFillRect(screen, &background_bar);

    // Vẽ thanh HP thực tế (màu xanh lá)
    SDL_Rect hp_bar = { x_pos, y_pos, hp_width, bar_height };
    SDL_SetRenderDrawColor(screen, 0, 255, 0, 255);  // Màu xanh lá
    SDL_RenderFillRect(screen, &hp_bar);

    // Hiển thị số máu còn lại
    if (font != NULL) {
        // Tạo chuỗi hiển thị máu (vd: "75/100")
        std::string hp_text = std::to_string(current_hp_) + " / " + std::to_string(max_hp_);

        // Tạo Surface từ text
        SDL_Color text_color = { 255, 255, 255, 255 };  // Màu trắng
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, hp_text.c_str(), text_color);
        if (text_surface != NULL) {
            // Tạo Texture từ Surface
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(screen, text_surface);

            // Xác định vị trí hiển thị text
            SDL_Rect text_rect;
            text_rect.x = x_pos;  // Hiển thị text cùng vị trí với thanh HP
            text_rect.y = y_pos - 15;  // Hiển thị text phía trên thanh HP
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;

            // Vẽ text lên màn hình
            SDL_RenderCopy(screen, text_texture, NULL, &text_rect);

            // Giải phóng Surface và Texture
            SDL_FreeSurface(text_surface);
            SDL_DestroyTexture(text_texture);
        }
    }
}


void BossObject::TakeDamage(int damage) {
    current_hp_ -= damage;
    if (current_hp_ < 0) {
        current_hp_ = 0;  // Không để HP âm
    }
}

void BossObject::DoPlayer(Map& g_map)
{
    if (think_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= PLAYER_SPEED;
        }

        else if (input_type_.right_ == 1)
        {
            x_val_ += PLAYER_SPEED;
        }

        if (input_type_.jump_ == 1)
        {
            if (on_ground_ == 1)
            {
                y_val_ = -PLAYER_HIGHT_VAL;
            }

            input_type_.jump_ = 0;
        }

        CheckToMap(g_map);

        //CenterEntityOnMap(g_map);
    }

    if (think_time_ > 0)
    {
        think_time_--;

        if (think_time_ == 0)
        {
            InitPlayer();
        }
    }
}

void BossObject::InitPlayer()
{
    x_val_ = 0;
    y_val_ = 0;

    if (x_pos_ > 256)
    {
        x_pos_ -= 256;
    }
    else
        x_pos_ = 0;
    y_pos_ = 0;
    think_time_ = 0;
    input_type_.left_ = 1;
}

void BossObject::CenterEntityOnMap(Map& g_map)
{
    g_map.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

    if (g_map.start_x_ < 0)
    {
        g_map.start_x_ = 0;
    }

    else if (g_map.start_x_ + SCREEN_WIDTH >= g_map.max_x_)
    {
        g_map.start_x_ = g_map.max_x_ - SCREEN_WIDTH;
    }

    g_map.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

    if (g_map.start_y_ < 0)
    {
        g_map.start_y_ = 0;
    }

    else if (g_map.start_y_ + SCREEN_HEIGHT >= g_map.max_y_)
    {
        g_map.start_y_ = g_map.max_y_ - SCREEN_HEIGHT;
    }
}
void BossObject::RemoveBullet(const int& idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
void BossObject::CheckToMap(Map& g_map)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    on_ground_ = 0;

    //Check Horizontal
    int height_min = height_frame_;//SDLCommonFunc::GetMin(height_frame_, TILE_SIZE);

 
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    // Check x1, x2 with full width of map
    // Check y1, y2 with full heigth of map
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // when object is moving by right  ===>
        {
            // Check current position of map. It is not blank_tile.
            if ((g_map.tile[y1][x2] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                // Fixed post of object at current post of map.
                // => Cannot moving when press button
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0; // cannot moving
            }
        }
        else if (x_val_ < 0) // When moving by left    <====
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y2][x1] != BLANK_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }


    // Check vertical
    int width_min = width_frame_;// SDLCommonFunc::GetMin(width_frame_, TILE_SIZE);

    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            //Similar for vertical
            if ((g_map.tile[y2][x1] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;

                y_val_ = 0;

                on_ground_ = 1;
            }
        }
        else if (y_val_ < 0)
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y1][x2] != BLANK_TILE))
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;

                y_val_ = 0;
            }
        }
    }

    //If there is not collision with map tile. 
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ >= g_map.max_x_)
    {
        x_pos_ = g_map.max_x_ - width_frame_ - 1;
    }

    if (y_pos_ > g_map.max_y_)
    {
        think_time_ = 60;
    }
}

void BossObject::InitBullet(SDL_Renderer* screen)
{
    if (current_hp_ <= 140 && current_hp_ >= 125 && !has_attacked_stage_140||
        current_hp_ <= 70&& current_hp_>=50&&!has_attacked_stage_70 ||
        current_hp_ <= 40 && current_hp_ >= 15 && !has_attacked_stage_40)
    {
        BulletObject* special_bullet = new BulletObject();
        bool special_ret = special_bullet->LoadImage("IMG/rasengan.png", screen); // Ảnh đạn đặc biệt
        if (special_ret)
        {
            
            Mix_Chunk* two_beep_chunk = Mix_LoadWAV(g_name_audio_rasengan_effect);
            if (two_beep_chunk != NULL)
            {
                Mix_PlayChannel(-1, two_beep_chunk, 0);
            }
            else {
               std:: cout << "fail";
            }
              
            special_bullet->set_bullet_dir(BulletObject::DIR_UP_LEFT); // Hướng bắn
            special_bullet->set_is_move(true);
            special_bullet->SetRect(rect_.x - 80, rect_.y + height_frame_ / 2); // Vị trí đạn đặc biệt
            special_bullet->set_x_val(20); // Tốc độ nhanh hơn đạn thường
            special_bullet->set_y_val(-5); // Đạn có quỹ đạo đi lên
            bullet_list_.push_back(special_bullet);
        }
    }
    BulletObject* p_bullet = new BulletObject();
    bool ret = p_bullet->LoadImage("IMG/boss bullet.png", screen);
    if (ret)
    {
        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->set_is_move(true);
        p_bullet->SetRect(rect_.x - 50, rect_.y + height_frame_ - 30);
        p_bullet->set_x_val(12);
        bullet_list_.push_back(p_bullet);
        
    }
    if (current_hp_ <= 70 && current_hp_ >= 50)
    {
        has_attacked_stage_70 = true;
    }
    else if (current_hp_ <= 40 && current_hp_ >= 15)
    {
        has_attacked_stage_40 = true;
    }
    else if (current_hp_ <= 140 && current_hp_ >= 125)
    {
        has_attacked_stage_140 = true;
    }
}


void BossObject::MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit)
{
    if (frame_ == 18)
    {
        InitBullet(des);
    }

    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet->Free();
                bullet_list_.erase(bullet_list_.begin() + i);
            }
        }
    }
}
void BossObject::Reset() {
    // Đặt lại máu và vị trí của boss
    
    x_pos_ = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH * 0.6;
    y_pos_ = 10;
    max_hp_ = 150;  // HP tối đa
    current_hp_ = 150;  // HP hiện tại
    // Các thiết lập khác nếu cần
    has_attacked_stage_140 = false;
    has_attacked_stage_70 = false;
    has_attacked_stage_40 = false;
    
    bullet_list_.clear();
}
