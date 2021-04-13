#include "popups/popup.hpp"

#include "modules/transpiler.hpp"

namespace CodeNect
{
void PopupRun::draw(void)
{
	m_is_inside = false;

	PopupRun::begin_draw();
	ImGui::Begin("Run", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		if (ImGui::MenuItem("Compile"))
		{
			Transpiler::compile();
			m_is_open = false;
		}

		if (ImGui::MenuItem("Run"))
		{
			Transpiler::run();
			m_is_open = false;
		}
	ImGui::End();

	PopupRun::end_draw();
}
}
