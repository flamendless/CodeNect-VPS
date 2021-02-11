#ifndef SIDEBAR_HANDLER_H
#define SIDEBAR_HANDLER_H

#include <cstdint>
#include <string>
#include "tweeny.h"
#include "sidebar.hpp"
#include "sidebar_indicator.hpp"

namespace CodeNect
{
struct SidebarHandler
{

static void init();
static void update(float dt);
};
}

#endif //SIDEBAR_HANDLER_H
