cmake_minimum_required(VERSION 3.7)

project(CodeNect LANGUAGES C CXX)
set(CMAKE_MODULE_PATH "cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(CodeNect
	# source files
	src/main.cpp
	src/core/app.cpp
	src/core/commands.cpp
	src/core/config.cpp
	src/core/font.cpp
	src/core/project.cpp
	src/core/utils.cpp
	src/core/validator.cpp
	src/modules/filesystem.cpp
	src/modules/input.cpp
	src/node/connection.cpp

	src/node/node_branch.cpp
	src/node/node_cast.cpp
	src/node/node_cmp.cpp
	src/node/node_colors.cpp
	src/node/node_math.cpp
	src/node/node.cpp
	src/node/node_logic_branch.cpp
	src/node/node_logic_cast.cpp
	src/node/node_logic_cmp.cpp
	src/node/node_logic.cpp
	src/node/node_logic_math.cpp
	src/node/node_logic_op.cpp
	src/node/node_logic_print.cpp
	src/node/node_logic_var.cpp
	src/node/node_action.cpp
	src/node/node_op.cpp
	src/node/node_print.cpp
	src/node/node_renderer.cpp
	src/node/nodes.cpp
	src/node/node_val.cpp
	src/node/node_var.cpp

	src/popups/popup.cpp
	src/popups/popup_project.cpp
	src/popups/popup_run.cpp

	src/ui/about.cpp
	src/ui/alert.cpp
	src/ui/command_palette.cpp
	src/ui/create_node_branch.cpp
	src/ui/create_node_cast.cpp
	src/ui/create_node_cmp.cpp
	src/ui/create_node.cpp
	src/ui/create_node_op.cpp
	src/ui/create_node_math.cpp
	src/ui/create_node_print.cpp
	src/ui/create_node_var.cpp
	src/ui/help.cpp
	src/ui/inspector.cpp
	src/ui/new_project.cpp
	src/ui/node_interface.cpp
	src/ui/settings.cpp
	src/ui/sidebar.cpp
	src/ui/sidebar_handler.cpp
	src/ui/sidebar_indicator.cpp

	# dearimgui
	includes/imgui/imgui.cpp
	includes/imgui/imgui_demo.cpp
	includes/imgui/imgui_draw.cpp
	includes/imgui/imgui_tables.cpp
	includes/imgui/imgui_widgets.cpp
	includes/imgui/backends/imgui_impl_opengl2.cpp
	includes/imgui/backends/imgui_impl_glfw.cpp
	# fmt
	includes/fmt/src/format.cc
	includes/fmt/src/os.cc
	# imnodes
	includes/imnodes/ImNodes.cpp
	includes/imnodes/ImNodesEz.cpp
	# simpleini
	includes/simpleini/ConvertUTF.c
	# nativefiledialogs
	includes/nfd/nfd_common.c
	# ppk_assert
	includes/ppk/src/ppk_assert.cpp
)

set_target_properties(CodeNect PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(CodeNect PRIVATE
	src
	# better-enums
	includes/better-enums
	# dearimgui
	includes/imgui
	includes/imgui/backends
	# fmt
	includes/fmt/include
	# iconfontcppheaders
	includes/iconfont
	# imnodes
	includes/imnodes
	# simpleini
	includes/simpleini
	# nativefiledialogs
	includes/nfd/include
	# plog
	includes/plog/include
	# ppk_assert
	includes/ppk/src
	# stb
	includes/stb
	# tweeny
	includes/tweeny/include
	# verbalexpressions
	includes/verbal_expr
	# 3rd-party
	${OPENGL_INCLUDE_DIR}
	${GLFW3_INCLUDE_DIR}
)

target_compile_definitions(CodeNect PRIVATE "$<$<CONFIG:Debug>:DEBUG_MODE=1>")

if (WIN32)
	target_compile_definitions(CodeNect PRIVATE OS_WIN=1)
	target_sources(CodeNect PRIVATE includes/nfd/nfd_win.cpp)
else ()
	target_compile_definitions(CodeNect PRIVATE OS_LINUX=1)

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	target_link_libraries(CodeNect ${GTK3_LIBRARIES} ${OPENGL_glx_LIBRARY})
	target_compile_options(CodeNect PRIVATE ${GTK3_CFLAGS_OTHER})
	target_link_directories(CodeNect PRIVATE ${GTK3_LIBRARY_DIRS})
	target_sources(CodeNect PRIVATE includes/nfd/nfd_gtk.c)
	target_include_directories(CodeNect PRIVATE ${GTK3_INCLUDE_DIRS})
endif ()

target_link_libraries(CodeNect
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
)

install(TARGETS CodeNect DESTINATION .)
install(FILES config.ini DESTINATION .)
install(DIRECTORY assets DESTINATION .)

add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}
    )