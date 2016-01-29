#find_package(CURL REQUIRED)
#find_package(ImageMagick REQUIRED COMPONENTS Magick++)

pkg_check_modules(SDL2 sdl2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW  REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

add_executable(lllll
			jsoncpp.cpp
			main.cpp
			
			ArtNetPacket.cpp ArtNetPacket.h
			Socket.cpp Socket.h
			Light.cpp Light.h
			DimmedLight.cpp DimmedLight.h
			Elar.cpp Elar.h
			Log.h
			
			GLHelper.cpp GLHelper.h
			GLShader.cpp GLShader.h
			GLProgram.cpp GLProgram.h
			Texture.cpp Texture.h
			Window.cpp Window.h
						
			)

include_directories(${SDL2_INCLUDE_DIRS} ${ImageMagick_Magick++_INCLUDE_DIR})
			
target_link_libraries(lllll 
				${SDL2_LIBRARIES} 
				${OPENGL_LIBRARIES} 
				${GLEW_LIBRARIES} 
				${ImageMagick_Magick++_LIBRARY}
				)
			
