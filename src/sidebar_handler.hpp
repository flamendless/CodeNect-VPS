#ifndef SIDEBAR_HANDLER_H
#define SIDEBAR_HANDLER_H
#define MS_SIDEBAR_TOGGLE 10

#include <cstdint>
#include <string>
#include "tweeny.h"
#include "sidebar.hpp"
#include "sidebar_indicator.hpp"

namespace CodeNect
{
struct SidebarHandler
{

static tweeny::tween<int> t_sidebar_x;
static tweeny::tween<int> t_sidebar_indicator_x;

static void init();
static void update(float dt);
};
}

#endif //SIDEBAR_HANDLER_H
