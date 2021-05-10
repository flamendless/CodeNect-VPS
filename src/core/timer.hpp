#ifndef _TIMER_HPP
#define _TIMER_HPP

namespace CodeNect
{
struct Timer
{
	float m_current_sec = 0;
	float m_max_sec = 0;
	bool m_repeat = false;
};
}

#endif //_TIMER_HPP
