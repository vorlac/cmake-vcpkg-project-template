#ifndef GUI_H
#define GUI_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef union SDL_Event SDL_Event;
    typedef struct SDL_Window SDL_Window;
    typedef void* SDL_GLContext;

    void gui_system_init(SDL_Window* window, SDL_GLContext glcontext, int width, int height);
    void gui_system_shutdown(void);

    void gui_system_begin(void);
    void gui_system_end(void);

    typedef struct gui_window_t gui_window_t;

    void gui_window_events(gui_window_t* gui, SDL_Event* event);
    void gui_window_update(gui_window_t* gui);
    void gui_window_render(gui_window_t* gui);

    void gui_window_resize(gui_window_t* gui, float width, float height);

#ifdef __cplusplus
}
#endif

#endif
