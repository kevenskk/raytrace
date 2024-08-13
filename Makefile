all:
	g++ -Isrc/include/SDL2 -Isrc/include/imgui -Lsrc/lib -o main main.cpp imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui_impl_sdlrenderer2.cpp imgui/imgui_impl_sdl2.cpp -lmingw32 -lSDL2main -lSDL2



