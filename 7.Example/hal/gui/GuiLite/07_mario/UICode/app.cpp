#define GUILITE_ON  // Do not define this macro once more!!!
#include "GuiLite.h"

#define UI_WIDTH  160
#define UI_HEIGHT 128

#define MARIO_WITH   16
#define MARIO_HEIGHT 32

#define FULL_STEP    9
#define MARIO_INIT_X 0
#define MARIO_INIT_Y (UI_HEIGHT - 1)
#define RYU_X        85
#define RYU_Y        90
#define ACC_X        0
#define ACC_Y        1


extern const BITMAP_INFO background_bmp, step1_bmp, step2_bmp, step3_bmp, jump_bmp;

static c_surface* s_surface;
static c_display* s_display;

class c_mario {
public:
    c_mario()
    {
        m_x          = MARIO_INIT_X;
        m_y          = MARIO_INIT_Y;
        m_step       = 0;
        m_is_jump    = false;
        m_x_velocity = 3;
        m_y_velocity = 0;
    }
    void jump()
    {
        m_is_jump    = true;
        m_y_velocity = -9;
    }
    void move()
    {
        if (m_step++ == FULL_STEP) { m_step = 0; }
        m_x_velocity += ACC_X;
        m_x += m_x_velocity;
        if (m_is_jump) {
            m_y_velocity += ACC_Y;
            m_y += m_y_velocity;
        }
        if ((m_x + MARIO_WITH - 1) >= UI_WIDTH) {
            m_x = 0;
        }
        if (m_y >= MARIO_INIT_Y) {
            m_y          = MARIO_INIT_Y;
            m_y_velocity = 0;
            m_is_jump    = false;
        }
        if (m_y < UI_HEIGHT - background_bmp.height + MARIO_HEIGHT) {
            m_y          = UI_HEIGHT - background_bmp.height + MARIO_HEIGHT;
            m_y_velocity = 0;
        }

        if (m_x == 93) {
            jump();
        }
    }
    void draw(bool erase_footprint)
    {
        const BITMAP_INFO* mario_bmp;
        if (m_is_jump) {
            mario_bmp = &jump_bmp;
        } else {
            if (m_step < (FULL_STEP / 3)) {
                mario_bmp = &step1_bmp;
            } else if (m_step < (FULL_STEP * 2 / 3)) {
                mario_bmp = &step2_bmp;
            } else {
                mario_bmp = &step3_bmp;
            }
        }

        if (erase_footprint) {
            c_rect mario_rect(m_x, m_y - mario_bmp->height, mario_bmp->width, mario_bmp->height);
            s_surface->show_layer(mario_rect, Z_ORDER_LEVEL_0);
        } else {
            c_image::draw_image(s_surface, Z_ORDER_LEVEL_1, mario_bmp, m_x, m_y - mario_bmp->height, GL_RGB(255, 255, 255));
        }
    }
    int          m_x, m_y, m_x_velocity, m_y_velocity;
    unsigned int m_step;
    bool         m_is_jump;
};

//////////////////////// start UI ////////////////////////
c_mario the_mario;

void create_ui(void* phy_fb, int screen_width, int screen_height, int color_bytes, struct DISPLAY_DRIVER* driver)
{
	  // surface 内存不够时，增加 heap 大小，减小 stack 大小，因为 calloc 用的是 heap
    static c_surface surface(UI_WIDTH, UI_HEIGHT, color_bytes, Z_ORDER_LEVEL_1,c_rect(0, UI_HEIGHT - background_bmp.height, UI_WIDTH, background_bmp.height));  
    static c_display display(phy_fb, screen_width, screen_height, &surface, driver);
    s_surface = &surface;
    s_display = &display;

		s_surface->fill_rect(0, 0, UI_WIDTH - 1, UI_HEIGHT - 1, GL_RGB(0,0,0xFF), Z_ORDER_LEVEL_0);
    c_image::draw_image(s_surface, Z_ORDER_LEVEL_0, &background_bmp, 0, UI_HEIGHT - background_bmp.height);

    while (1) {
        the_mario.draw(true);
        the_mario.move();
        the_mario.draw(false);
        thread_sleep(50);
    }
}


//////////////////////// interface for all platform ////////////////////////
extern "C" void startHelloMario(void* phy_fb, int color_bytes, struct DISPLAY_DRIVER* driver)
{
    create_ui(phy_fb, UI_WIDTH, UI_HEIGHT, color_bytes, driver);
}

extern "C" void* getUiOfHelloMario(int* width, int* height, bool force_update)
{
    return s_display->get_updated_fb(width, height, force_update);
}

void sendTouch2HelloMario(int x, int y, bool is_down)
{
    if (is_down) { the_mario.jump(); }
}
