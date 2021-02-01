target("CodeNect")
	set_kind("binary")

	--source files
	add_files("src/*.cpp")
	add_includedirs("src")

	--dearimgui
	add_files("includes/imgui/*.cpp", "includes/imgui/backends/imgui_impl_opengl2.cpp", "includes/imgui/backends/imgui_impl_sdl.cpp")
	add_includedirs("includes/imgui", "includes/imgui/backends")

	--imnodes
	add_files("includes/imnodes/*.cpp|sample.cpp")
	add_includedirs("includes/imnodes")

	--fmt
	add_files("includes/fmt/src/*.cc")
	add_includedirs("includes/fmt/include")

	add_cxxflags("-g", "-Wall", "-Wformat", "-pedantic", "$(shell sdl2-config --cflags)")
	add_ldflags("$(shell sdl2-config --libs)")

	if is_mode("debug") then
		add_defines("DEBUG_MODE=1")
		set_symbols("debug")
		set_optimize("none")
	end

	if is_mode("release") then
		set_symbols("hidden")
		set_optimize("fastest")
		set_strip("all")
	end

	on_load(function(target)
		local packages = find_packages("GL", "dl")

		target:add(packages)
	end)
