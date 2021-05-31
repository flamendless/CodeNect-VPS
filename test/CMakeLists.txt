cmake_minimum_required(VERSION 3.7)

project(CodeNectTest LANGUAGES C CXX)
set(CMAKE_MODULE_PATH "../cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -pedantic")

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(CodeNectTest
	# source files
	test.cpp
	../src/core/app.cpp
	../src/core/commands.cpp
	../src/core/config.cpp
	../src/core/font.cpp
	../src/core/markdown.cpp
	../src/core/project.cpp
	../src/core/utils.cpp
	../src/core/validator.cpp
	../src/modules/assessments.cpp
	../src/modules/debugger.cpp
	../src/modules/filesystem.cpp
	../src/modules/input.cpp
	../src/modules/node_to_code.cpp
	../src/modules/simulation.cpp
	../src/modules/transpiler.cpp
	../src/modules/transpiler_code.cpp
	../src/node/connection.cpp

	../src/node/node.cpp
	../src/node/node_array.cpp
	../src/node/node_array_access.cpp
	../src/node/node_branch.cpp
	../src/node/node_cast.cpp
	../src/node/node_cmp.cpp
	../src/node/node_colors.cpp
	../src/node/node_for.cpp
	../src/node/node_math.cpp
	../src/node/node_op.cpp
	../src/node/node_print.cpp
	../src/node/node_prompt.cpp
	../src/node/node_renderer.cpp
	../src/node/node_set.cpp
	../src/node/node_size.cpp
	../src/node/node_string.cpp
	../src/node/node_val.cpp
	../src/node/node_var.cpp
	../src/node/nodes.cpp

	../src/node/node_logic_array.cpp
	../src/node/node_logic_array_access.cpp
	../src/node/node_logic_branch.cpp
	../src/node/node_logic_cast.cpp
	../src/node/node_logic_cmp.cpp
	../src/node/node_logic.cpp
	../src/node/node_logic_for.cpp
	../src/node/node_logic_math.cpp
	../src/node/node_logic_op.cpp
	../src/node/node_logic_print.cpp
	../src/node/node_logic_prompt.cpp
	../src/node/node_logic_process_branch.cpp
	../src/node/node_logic_set.cpp
	../src/node/node_logic_size.cpp
	../src/node/node_logic_string.cpp
	../src/node/node_logic_var.cpp

	../src/popups/popup.cpp
	../src/popups/popup_project.cpp
	../src/popups/popup_run.cpp

	../src/ui/about.cpp
	../src/ui/alert.cpp
	../src/ui/assessments.cpp
	../src/ui/button.cpp
	../src/ui/command_palette.cpp
	../src/ui/create_node_array.cpp
	../src/ui/create_node_array_access.cpp
	../src/ui/create_node_branch.cpp
	../src/ui/create_node_cast.cpp
	../src/ui/create_node_cmp.cpp
	../src/ui/create_node.cpp
	../src/ui/create_node_op.cpp
	../src/ui/create_node_for.cpp
	../src/ui/create_node_math.cpp
	../src/ui/create_node_print.cpp
	../src/ui/create_node_prompt.cpp
	../src/ui/create_node_set.cpp
	../src/ui/create_node_size.cpp
	../src/ui/create_node_string.cpp
	../src/ui/create_node_var.cpp
	../src/ui/diff_viewer.cpp
	../src/ui/docs.cpp
	../src/ui/help.cpp
	../src/ui/inspector.cpp
	../src/ui/new_project.cpp
	../src/ui/node_interface.cpp
	../src/ui/settings.cpp
	../src/ui/sidebar.cpp
	../src/ui/sidebar_handler.cpp
	../src/ui/sidebar_indicator.cpp
	../src/ui/simulation_control.cpp
	../src/ui/terminal.cpp
	../src/ui/zoom.cpp

	# color text editor
	../includes/color_text_edit/TextEditor.cpp
	# dearimgui
	../includes/imgui/imgui.cpp
	../includes/imgui/imgui_draw.cpp
	../includes/imgui/imgui_tables.cpp
	../includes/imgui/imgui_widgets.cpp
	../includes/imgui/backends/imgui_impl_opengl2.cpp
	../includes/imgui/backends/imgui_impl_glfw.cpp
	# fmt
	../includes/fmt/src/format.cc
	../includes/fmt/src/os.cc
	# imnodes
	../includes/imnodes/ImNodes.cpp
	../includes/imnodes/ImNodesEz.cpp
	# simpleini
	../includes/simpleini/ConvertUTF.c
	# nativefiledialogs
	../includes/nfd/nfd_common.c
	# ppk_assert
	../includes/ppk/src/ppk_assert.cpp
)

set_target_properties(CodeNectTest PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(CodeNectTest PRIVATE
	../src
	../src/core/
	# better-enums
	../includes/better-enums
	# color text editor
	../includes/color_text_edit
	# dearimgui
	../includes/imgui
	../includes/imgui/backends
	# fmt
	../includes/fmt/include
	# iconfontcppheaders
	../includes/iconfont
	#imgui_markdown
	../includes/imgui_markdown
	# imnodes
	../includes/imnodes
	# simpleini
	../includes/simpleini
	# nativefiledialogs
	../includes/nfd/include
	# plog
	../includes/plog/include
	# ppk_assert
	../includes/ppk/src
	# stb
	../includes/stb
	# tweeny
	../includes/tweeny/include
	# verbalexpressions
	../includes/verbal_expr
	#tcc
	../includes/tcc
	#doctest
	doctest/doctest/
	# 3rd-party
	${OPENGL_INCLUDE_DIR}
	${GLFW3_INCLUDE_DIR}
)

target_compile_definitions(CodeNectTest PRIVATE OS_LINUX=1)
target_compile_definitions(CodeNectTest PRIVATE "$<$<CONFIG:Debug>:DEBUG_MODE=1>")
target_compile_definitions(CodeNectTest PRIVATE "$<$<CONFIG:Debug>:TEST=1>")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

if (WIN32)
	target_compile_definitions(CodeNectTest PRIVATE OS_WIN=1)
	target_sources(CodeNectTest PRIVATE ../includes/nfd/nfd_win.cpp)
	target_link_libraries(CodeNectTest ${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows/libtcc1-64.a)
	target_link_libraries(CodeNectTest ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libtcc.dll)
	install(FILES
			../lib/libtcc.dll
		DESTINATION .)
	install(FILES
			../lib/windows/libtcc1-64.a
		DESTINATION ./lib)
else ()
	target_compile_definitions(CodeNectTest PRIVATE OS_LINUX=1)

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	target_link_libraries(CodeNectTest ${GTK3_LIBRARIES} ${OPENGL_glx_LIBRARY})
	target_compile_options(CodeNectTest PRIVATE ${GTK3_CFLAGS_OTHER})
	target_link_directories(CodeNectTest PRIVATE ${GTK3_LIBRARY_DIRS})
	target_sources(CodeNectTest PRIVATE ../includes/nfd/nfd_gtk.c)
	target_include_directories(CodeNectTest PRIVATE ${GTK3_INCLUDE_DIRS})
	target_link_libraries(CodeNectTest ${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/libtcc.a)
	target_link_libraries(CodeNectTest ${CMAKE_CURRENT_SOURCE_DIR}/../lib/linux/libtcc1.a)
	target_link_libraries(CodeNectTest dl)
endif ()

target_link_libraries(CodeNectTest
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
)
install(TARGETS CodeNectTest DESTINATION .)
install(DIRECTORY ../lib/tcc DESTINATION .)
install(FILES
		../lib/linux/libtcc.a
	DESTINATION .)
install(FILES
		../lib/linux/libtcc.a
		../lib/linux/libtcc1.a
	DESTINATION ./lib)
install(FILES
		../lib/glfw3.dll
		../lib/libgcc_s_seh-1.dll
		../lib/libstdc++-6.dll
		../lib/libwinpthread-1.dll
	DESTINATION .)

add_custom_target(
	copy-compile-commands ALL
)
