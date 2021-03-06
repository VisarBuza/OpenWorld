cmake_minimum_required (VERSION 3.14 FATAL_ERROR)
project(Exam VERSION 0.0.1)

# Add dependencies
add_subdirectory(external)

# Create .exe file (essentially)
add_executable(${PROJECT_NAME})

# Find freetype
find_package(Freetype REQUIRED)

# Add additional source files here
target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/util.h
    ${CMAKE_CURRENT_LIST_DIR}/include/config.h
    # ${CMAKE_CURRENT_LIST_DIR}/include/shader.h
    # ${CMAKE_CURRENT_LIST_DIR}/include/camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/text_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/rendering/text_renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/game_object.h
    ${CMAKE_CURRENT_LIST_DIR}/src/game_object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/terrain.h
    ${CMAKE_CURRENT_LIST_DIR}/src/terrain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/route_planner.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ai/route_planner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/game.h
    ${CMAKE_CURRENT_LIST_DIR}/src/game.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/include/model.h
    ${CMAKE_CURRENT_LIST_DIR}/src/rendering/model.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/src/rendering/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_widgets.cpp
)

# Set include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${FREETYPE_INCLUDE_DIRS}
)

# Link with external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glad::glad
    gfx::gfx
    glm
    glfw
    stbimage::stbimage
    tinyobj::tinyobj
    ${FREETYPE_LIBRARIES}
)

# Enable C++ 17
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)

# Copy resources to binary directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E echo "Copied resource directory to binary directory."
)

