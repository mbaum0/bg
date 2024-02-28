# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.7)

project(sage)

# Add SDL2 CMake modules
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Define the library
add_library(${PROJECT_NAME} STATIC 
    src/sageutil.c
    src/vmanager.c
    src/sprite.c
    src/snippet.c
    src/events.c
    src/vector.c
    src/mmanager.c
    src/log.c
    src/sage.c
    src/input.c
    src/bmfont.c
)

# Specify include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC include)

# # Add SDL2 library
# find_package(SDL2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::Main)

# # Add SDL2_image library
# find_package(SDL2_image REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::Image)

# # Add SDL2_ttf library
# find_package(SDL2_ttf REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::TTF)

# add the SDL3 library
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

# find_package(SDL3_ttf REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL3_ttf::SDL3_ttf)

find_package(SDL3_image REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL3_image::SDL3_image)

# Add math
target_link_libraries(${PROJECT_NAME} m)
