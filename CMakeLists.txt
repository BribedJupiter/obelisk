####### Obelisk v0.02 #######
cmake_minimum_required(VERSION 3.28)
project(obelisk LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include(FetchContent)
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(glm)

if (APPLE)
    add_executable(main src/main.cpp src/obShader.cpp src/obCamera.cpp lib/glad_macos/src/glad.c lib/stb/stb_impl.cpp)
    target_compile_definitions(main PRIVATE IS_MACOS)
    target_include_directories(main PRIVATE lib/glad_macos/include PRIVATE lib/glad_macos/KHR)
else()
    add_executable(main src/main.cpp src/obShader.cpp src/obCamera.cpp lib/glad_windows/src/glad.c lib/stb/stb_impl.cpp)
    target_include_directories(main PRIVATE lib/glad_windows/include PRIVATE lib/glad_windows/KHR)
endif()

target_include_directories(main PRIVATE src PRIVATE lib/stb/include)
target_compile_features(main PRIVATE cxx_std_17)
target_compile_definitions(main PRIVATE
    SHADER_PATH="${CMAKE_SOURCE_DIR}/shaders"
    TEXTURE_PATH="${CMAKE_SOURCE_DIR}/textures"
)

target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network glm::glm)
