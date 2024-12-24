#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>
#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include"PlayerPower.h"
#define GRAVITY_SPEED 0.8 // trọng lượng rơi
#define MAX_FALL_SPEED 10 // MAX giới hạn rơi
#define PLAYER_SPEED 15//  tốc độ chạy
#define PLAYER_JUMP_VAL 15

class MainObject : public BaseObject {
public:
    static const int FRAME = 8; // Số lượng frame trong hoạt ảnh

    MainObject();  // Constructor
    ~MainObject(); // Destructor

    enum WalkType {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
        WALK_UP = 3,
        SPECIAL = 4,
        HEALPOINT = 5,
    };
    enum typeBullet
    {
        NORMAL=100,
        CHIDORI=101,
    };
    // Tải hình ảnh cho đối tượng
    bool LoadImage(std::string path, SDL_Renderer* screen);

  

    // Hiển thị đối tượng
    void Show(SDL_Renderer* des);

    // Xử lý hành động nhập liệu
    void MandelInputAction(SDL_Event events, SDL_Renderer* screen);

    // Thiết lập các clip cho hoạt ảnh
    void set_clips();

    // cộng trừ cho nhân vật di chuyển
    void DoPlayer(Map& map_data,PlayerPower&playerPower,int &numdie);
    // Kiểm tra sự va chạm
    void CheckToMap(Map& map_data, PlayerPower& playerPower, int& numdie);

    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x;  map_y_ = map_y; }//4.3
    void CenterEntityOnMap(Map& map_data);
  

    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();

    void set_bullet_list(std::vector<BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
  
    std::vector<BulletObject*>get_bullet_list()const { return p_bullet_list_; }

    void HandleBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);
    void IncreaseMoney();
    int get_frame_width() const { return width_frame_; }
    int get_frame_height() const { return height_frame_; }
    void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }

    int GetMoneyCount()const { return money_count; }
    int get_status() { return status_; }
    void set_status(int status) {
        this->status_ = status;
        std::cout << "status_ " << status_;
    }

    void Reset();
    void set_input_type(const Input& input) {
        input_type_ = input;
    }
    void UpdateHP(PlayerPower &playerpower,int &num_die)
    {
        if (limhp < 4)
        {
            if (num_die > 0)
            {
                if (money_count >= 10 && status_ == HEALPOINT)
                {
                    playerpower.InitCrease();
                    num_die--;
                    money_count = money_count - 10;
                    limhp++;
                }
            }
        }
       
       
      
    }
    void set_type_bullet(int& typeBullet)
    {
        typeBullet=type_bullet;
    }
    int get_comback_time()
    {
        return come_back_time_;
    }
    void UpdateComebackTime(PlayerPower&playerPower)
    {
        if (come_back_time_ == 60)
        {
            playerPower.Decrease();
            playerPower.Render(g_screen);
        }
    }
    void set_hp(int& numdie)
    {
        hp = numdie;
    }
    void set_status_voice(bool& voice_status_) { voice_status_ = status_voice; }
    
private:
    int type_bullet;
    int limhp;
    int hp;
    int money_count;
    std::vector<BulletObject*>p_bullet_list_;
    float x_val_;          // Giá trị di chuyển theo trục x
    float y_val_;          // Giá trị di chuyển theo trục y
    float x_pos_;          // Vị trí hiện tại theo trục x
    float y_pos_;          // Vị trí hiện tại theo trục y
    int width_frame_;      // Chiều rộng của mỗi frame
    int height_frame_;     // Chiều cao của mỗi frame
    SDL_Rect frame_clip_[FRAME]; // Mảng chứa các clip frame
    Input input_type_;     // Thông tin nhập liệu
    int frame_;            // Chỉ số frame hiện tại
    int status_;           // Trạng thái hiện tại (đi sang trái, phải)
    bool on_ground_;
    
    int map_x_;
    int map_y_;

    int come_back_time_;
    bool status_voice;
    Uint32 chidori_last_used_;
    
};


#endif // MAIN_OBJECT_H_
