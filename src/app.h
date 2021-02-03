#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "fmt/printf.h"
#include "plog/Init.h"
#include "plog/Log.h"

#if DEBUG_MODE
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#else
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include "config.h"


namespace App
{
static SDL_Window* window;
static SDL_GLContext gl_context;
static ImGuiIO* imgui_io;

int init();
void init_window();
void init_imgui();
void update(bool& is_running);
void render_start();
void render_end();
void shutdown();
}

#endif //APP_H
