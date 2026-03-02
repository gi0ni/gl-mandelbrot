cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_DIR ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

set(CMAKE_BUILD_TYPE Debug)

# proj
file(GLOB SRCS "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRCS})

if(LINUX)
	set(GL_PATH "/home/gi0ni/lib")
	set(LINK_DIR "lib" )
elseif(WIN32)
	set(GL_PATH "/lib")
	set(LINK_DIR "lib-mingw-w64")
endif()

# utils
file(GLOB UTILS "${GL_PATH}/utils/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${UTILS})
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/utils/include/")

# glad
target_sources(${PROJECT_NAME} PRIVATE "${GL_PATH}/glad/src/glad.c")
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/glad/include/")

# glfw
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/glfw/include/")
target_link_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/glfw/${LINK_DIR}/")
if(LINUX)
	target_link_libraries(${PROJECT_NAME} -lglfw)
elseif(WIN32)
	target_link_libraries(${PROJECT_NAME} -lglfw3)
endif()

# stb
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/stb/include/")

# glm
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/glm/include/")

# imgui
file(GLOB IMGUI "${GL_PATH}/imgui/src/*.cpp")
file(GLOB IMGUI_BACKEND "${GL_PATH}/imgui/src/backends/OpenGL/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI} ${IMGUI_BACKEND})
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/imgui/include/")

# implot
file(GLOB IMPLOT "${GL_PATH}/implot/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${IMPLOT})
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/implot/include/")

# assimp
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/assimp/include/")
target_link_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/assimp/${LINK_DIR}/")
target_link_libraries(${PROJECT_NAME} -lassimp)

# freetype
target_include_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/freetype/include/")
target_link_directories(${PROJECT_NAME} PRIVATE "${GL_PATH}/freetype/${LINK_DIR}/")
target_link_libraries(${PROJECT_NAME} -lfreetype)
