#find_package(CURL REQUIRED)
#find_package(ImageMagick REQUIRED COMPONENTS Magick++)

pkg_check_modules(SDL2 sdl2 REQUIRED)

add_executable(lllll 
			main.cpp
			Log.h
			Window.cpp Window.h
			ArtNetPacket.cpp ArtNetPacket.h
			Socket.cpp Socket.h
			)

include_directories(${SDL2_INCLUDE_DIRS})			
target_link_libraries(lllll ${SDL2_LIBRARIES})
			
