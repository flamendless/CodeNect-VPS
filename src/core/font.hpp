#ifndef FONT_HPP
#define FONT_HPP

#include <string>

namespace CodeNect
{
struct Font
{
	static const char* far_path;
	static const char* fas_path;
	static const char* fab_path;
	static const char* proggy_clean_path;
	static const char* proggy_tiny_path;
	static const char* droid_sans_path;
	static const char* cousine_regular_path;
	static const char* karla_regular_path;
	static const char* roboto_medium_path;

	static void init(void);
};
}

#endif //FONT_HPP
