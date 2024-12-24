#include "Button.h"

Button::Button()
{
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 200;
    rect_.h = 60;
    text_ = "";
    color_ = { 255, 255, 255 };
    font_ = TTF_OpenFont("FONT/game_font.ttf", 28);
}

Button::~Button()
{
    TTF_CloseFont(font_);
}

void Button::SetPosition(int x, int y)
{
    rect_.x = x;
    rect_.y = y;
}

void Button::SetText(std::string text)
{
    text_ = text;
}

void Button::Render(SDL_Renderer* screen)
{
    // V? khung nút
    SDL_SetRenderDrawColor(screen, 0, 0, 255, 255); // Màu xanh
    SDL_RenderFillRect(screen, &rect_);

    // V? Text
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text_.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(screen, textSurface);

    SDL_Rect textRect;
    textRect.x = rect_.x + (rect_.w - textSurface->w) / 2;
    textRect.y = rect_.y + (rect_.h - textSurface->h) / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(screen, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

bool Button::HandleEvent(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= rect_.x && x <= rect_.x + rect_.w && y >= rect_.y && y <= rect_.y + rect_.h)
        {
            return true;
        }
    }
    return false;
}
