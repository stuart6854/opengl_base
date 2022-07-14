add_library(imgui 
	imgui.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	imgui_tables.cpp
	imgui_demo.cpp
)

target_include_directories(imgui PUBLIC ".")