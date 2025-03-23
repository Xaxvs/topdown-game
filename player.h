
#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>

enum item_type {
    SHIELD,
    WEAPON
};

struct item {
    char* name;
    int damage;
    int cost;
    item_type type;
};

class player {

    explicit player(const char* display_name):
    m_display_name(display_name),
    m_weapon({"Default Knife", 10, 5, WEAPON}),
    m_shield({"Default Chestplate", 5, 5, SHIELD})
    {

    }

public:
    const char* get_display_name() const;
    void update(KeyboardKey key_pressed);
    item& get_weapon() const;
    item& get_shield() const;
    void set_weapon(const item& weapon) const;
    void set_shield(const item& shield) const;
    int get_x() const;
    int get_y() const;
    int set_x(int value);
    int set_y(int value);

private:
    const char* m_display_name;
    item& m_weapon;
    item& m_shield;
    int m_x{}, m_y{};
};



#endif //PLAYER_H
