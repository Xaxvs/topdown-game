
#ifndef BUTTON_H
#define BUTTON_H
#include <complex>
#include <raylib.h>
#include <raylib-nuklear.h>

class Button {

public:

    Button(const char* texture_path, const float x, const float y,
        const float width, const float height, Color color):
    m_rect({x,y,width,height}), m_texture(LoadTexture(texture_path)),
    m_text(), m_texture_set(true),
    m_x(x), m_y(y),
    m_color(color), m_text_color(),
    m_font_size(0)
    {
    }

    Button(const char* text, const Color& text_color, const int font_size, const float x,
        const float y, const float width, const float height, Color color):
    m_rect({x,y,width,height}), m_texture(LoadTexture("null")),
    m_text(text), m_texture_set(false),
    m_x(x), m_y(y),
    m_color(color), m_text_color(text_color),
    m_font_size(font_size)
    {
    }

public:
    void Update() const {

        if(m_texture_set) {
            DrawTextureRec(m_texture, m_rect, {m_x, m_y}, m_color);
        }else {
            DrawRectangleRec(m_rect, m_color);
            DrawText(m_text, /*std::abs(m_x - m_rect.width) / 2*/ m_x , /*std::abs(m_y - m_rect.height) / 2*/ m_y, m_font_size, m_text_color);
        }

    }

    const Rectangle& GetRect() const {
        return m_rect;
    }

    bool TextureSet() const {
        return m_texture_set;
    }

private:
    Rectangle m_rect;
    const Texture2D& m_texture;
    const char* m_text;
    bool m_texture_set;
    float m_x, m_y;
    Color m_color, m_text_color;
    int m_font_size;
};

#endif //BUTTON_H
