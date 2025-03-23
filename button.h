
#ifndef BUTTON_H
#define BUTTON_H
#include <raylib.h>

class button {

    button(const char* texture_path, const char* text, const Color& text_color, float x, float y, int width, int height, Color color):
    m_rect({x,y,width,height}), m_texture(LoadTexture(texture_path)),
    m_text(text), m_texture_set(true),
    m_x(x), m_y(y),
    m_color(color), m_text_color(text_color)
    {

    }

    button(const char* text, const Color& text_color, float x, float y, int width, int height, Color color):
    m_rect({x,y,width,height}), m_texture(),
    m_text(text), m_texture_set(false),
    m_x(x), m_y(y),
    m_color(color), m_text_color(text_color)
    {

    }

public:
    void update() {

        if(m_texture_set) {
            DrawTextureRec(m_texture, m_rect, {m_x, m_y}, m_color);
        }else {
            DrawRectangleRec(m_rect, m_color);
        }

    }

    Rectangle& get_rect() const {
        return m_rect;
    }

    bool is_texture_set() const {
        return m_texture_set;
    }

private:
    Rectangle& m_rect;
    const Texture2D& m_texture;
    const char* m_text;
    bool m_texture_set;
    float m_x, m_y;
    Color& m_color, m_text_color;
};

#endif //BUTTON_H
