cmake_minimum_required(VERSION 3.15.0...3.30.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(
        /W4
        /Zi

        /nologo
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        #-Werror
    )
endif()

set(CMAKE_GENERATOR_PLATFORM x64)
# set(CMAKE_GENERATOR_PLATFORM win32)


project(spinny_cube_go_brrrrrrrrrrrrrrr)
    set(
        PROJECT_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set(
        PROJECT_SRC_DIRECTORY
        ${PROJECT_DIRECTORY}/IG1App
    )
    set(
        PROJECT_LIB_DIRECTORY
        ${PROJECT_DIRECTORY}/libs
    )
    file(
        GLOB_RECURSE
        PROJECT_SRC_FILES
        CMAKE_CONFIGURE_DEPENDS
        ${PROJECT_SRC_DIRECTORY}/*.cpp
    )

    set(
        GLM_DIRECTORY
        ${PROJECT_LIB_DIRECTORY}/glm
    )
    file(
        GLOB_RECURSE
        GLM_SRC_FILES
        CMAKE_CONFIGURE_DEPENDS
        ${GLM_DIRECTORY}/include/glm/*.cpp
    )

    add_executable(
        ${PROJECT_NAME}
        ${PROJECT_SRC_DIRECTORY}/main.cpp
        ${GLM_SRC_FILES}
        ${PROJECT_SRC_FILES}
    )

    set(
        GLEW_DIRECTORY
        ${PROJECT_LIB_DIRECTORY}/glew
    )
    set(
        GLFW_DIRECTORY
        ${PROJECT_LIB_DIRECTORY}/glfw
    )
    set(
        STB_IMAGE_DIRECTORY
        ${PROJECT_LIB_DIRECTORY}/stb_image
    )

    target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SRC_DIRECTORY}
    )
    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${GLEW_DIRECTORY}/include
        ${GLFW_DIRECTORY}/include
        ${GLM_DIRECTORY}/include
        ${STB_IMAGE_DIRECTORY}/include
    )

    target_link_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${GLEW_DIRECTORY}/lib
        ${GLFW_DIRECTORY}/lib
    )
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        glew32
        glfw3dll
        opengl32
    )
    set(
        PROJECT_EXE_DIRECTORY
        ${PROJECT_DIRECTORY}/build/x64-${CMAKE_BUILD_TYPE}/${CMAKE_BUILD_TYPE}
    )
    set(
        DLL_DIRECTORIES
        ${GLEW_DIRECTORY}/bin
        ${GLFW_DIRECTORY}/bin
    )

    add_custom_target(
        copy_resources_bin
        COMMAND ${CMAKE_COMMAND}
            -D V_PROJECT_DIRECTORY=${PROJECT_DIRECTORY}
            -D V_EXECUTABLE_DIRECTORY=${PROJECT_EXE_DIRECTORY}
            "-D V_DLL_DIRECTORIES=${DLL_DIRECTORIES}"
            -P ${CMAKE_CURRENT_LIST_DIR}/copy-resources.cmake
    )
    add_dependencies(${PROJECT_NAME} copy_resources_bin)