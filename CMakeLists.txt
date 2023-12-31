
# TODO: QTGUI 브랜치의 cmake을 참고해서 내용 정리할 것.

# CMAKE SETTING
# -------------------------------------------------
# https://cmake.org/cmake/help/latest/guide/tutorial/index.html
cmake_minimum_required(VERSION 3.24) # set cmake version
# https://cmake.org/cmake/help/latest/command/set.html
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE) # close DEPRECATION WARNING...
# include cmake custom util functions (ex. color message)
include( ${CMAKE_SOURCE_DIR}/utils.cmake ) # custom print_message() function.

# run build_dependency.sh script before compilation (ex. GLEW, GLFW)
# ----------------------------------------------------------------------------
#ADD_CUSTOM_COMMAND(
#        TARGET ${NAME}
#        COMMAND ${CMAKE_SOURCE_DIR}/dependency/build_dependency.sh
#        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # where CMakeLists.txt is located
#        COMMENT "${BoldGreen}Building project dependency binaries [GLEW, GLFW]...${ColourReset}"
#)

# set variable
# ********************************************
set( DEBUG_MODE 1 ) # DEBUG MODE --> use Log
#if( DEFINED TBB) # if CPU in Intel, then build_dependency script will set TBB variable (true)
    set ( MT 1 )
#    print_message( STATUS "Intel TBB Multi-threading is ON" ) # path set by "find_package"
#endif ()

# *******************************************

set( NAME scoop )
set( CMAKE_CXX_STANDARD 20 ) # Cpp compiler version
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( SOURCE_FILES
    # ImGui
        Lunar/ImGui/ImGuiBuild.cpp # ImGui Source
    # Project
        LunarApp/src/App.cpp
        LunarApp/src/Shaders/DisplayMode.cpp
        # Lunar Core
        Lunar/Core/Application.cpp
        Lunar/Input/Input.cpp
        Lunar/Shader/Shader.cpp
        Lunar/Mesh/Mesh.cpp
#        Lunar/Camera/PlayerCamera.cpp # Player's view
        Lunar/Camera/EditorCamera.cpp # Editor View
        Lunar/Texture/Texture.cpp
        Lunar/Model/Model.cpp
        Lunar/Light/Light.cpp
        Lunar/Material/Material.cpp
        Lunar/FrameBuffer/FrameBuffer.cpp
        Lunar/AABB/Mesh.cpp
        LunarApp/src/RayTracer/RayTracer.cpp
        )
# configure cmake project
# ----------------------------------------------------------------------------
project( ${NAME} ) # set project name
add_executable( ${NAME} ${SOURCE_FILES} )

# Set cmake to add define variable to specific target.
# ----------------------------------------------------------------------------
# 이 행위를 통해, 실행 파일 내부 에서 fopen등 함수 사용시 CMakelist.txt 파일 기준 절대경로 설정하기 위함.
target_compile_definitions( ${NAME}
        PRIVATE
        DEBUG_MODE # SPDLOG activation
        MT  # TBB multi-threading
        PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
        GLM_FORCE_SWIZZLE # force GLM to report the configuration as part of the build log
        )

# OpenGL
# ----------------------------------------------------------------------------
# 시스템에서 패키지를 찾고, 그 위치와 바이너리 이름들을 변수로 자동 저장해준다.
find_package( OpenGL REQUIRED ) # stops processing with an error message if the package cannot be found.
set( OpenGL_GL_PREFERENCE LEGACY )
print_message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIR}") # path set by "find_package"

# GLM math library
# ----------------------------------------------------------------------------
# https://github.com/g-truc/glm/blob/master/manual.md#-15-finding-glm-with-cmake
set( glm_DIR ${CMAKE_SOURCE_DIR}/dependency/glm/cmake/glm ) # if necessary
find_package( glm CONFIG REQUIRED )
target_link_libraries( ${NAME} PRIVATE glm::glm )
print_message( STATUS "GLM included at ${GLM_INCLUDE_DIRS}" ) # path set by "find_package"

# ImGui
# ----------------------------------------------------------------------------



# spdlog logging library
# ----------------------------------------------------------------------------
# https://github.com/gabime/spdlog/blob/v1.x/example/CMakeLists.txt
set(spdlog_DIR ${CMAKE_SOURCE_DIR}/dependency/spdlog/build) # if necessary
find_package( spdlog CONFIG REQUIRED )
target_link_libraries( ${NAME} PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> )
print_message( STATUS "SpdLog included at ${CMAKE_SOURCE_DIR}/dependency/spdlog" ) # path set by "find_package"


set (BREW_LIBRARY_INCLUDE_PATH "$(brew --prefix)/include")

# set include path for GLEW, GLFW
# ----------------------------------------------------------------------------
include_directories( SYSTEM # to use '#include <...>'
        ${BREW_LIBRARY_PATH}/include

        ${CMAKE_SOURCE_DIR}/dependency/GLEW/include
        ${CMAKE_SOURCE_DIR}/dependency/GLFW/include
        ${CMAKE_SOURCE_DIR}/dependency/glm/glm
        ${CMAKE_SOURCE_DIR}/dependency/stb_image
        ${CMAKE_SOURCE_DIR}/dependency/spdlog/include

    # TODO: Delete Later !! *************************
        ${CMAKE_SOURCE_DIR}/dependency/assimp/include
        ${CMAKE_SOURCE_DIR}/dependency/assimp/build/include
    # TODO: *****************************************
        ${CMAKE_SOURCE_DIR}/dependency/imgui
        )

# TODO: Include Lunar
include_directories(
        ${CMAKE_SOURCE_DIR}
        )


# link libraries (archive files)
# ----------------------------------------------------------------------------
target_link_libraries( ${NAME}
        PRIVATE # --> [?] : 참고 https://cmake.org/pipermail/cmake/2016-May/063400.html
        OpenGL::GL # 이렇게 해야 gl-native 함수 (Ex. glXGetProcAddressARB)를 링크할 수 있다.
        # -lGL --> (find_package 명령어와 OpenGL::GL이 대신함)
        ${CMAKE_SOURCE_DIR}/dependency/GLEW/build/lib/libGLEW.a
        ${CMAKE_SOURCE_DIR}/dependency/GLFW/build/src/libglfw3.a
        )

# TODO: Delete Later !! **************************************************
if(CMAKE_SYSTEM_NAME STREQUAL "Linux") # Linux
    target_link_libraries( ${NAME}
            PRIVATE
            ${CMAKE_SOURCE_DIR}/dependency/assimp/build/bin/libassimp.so
            )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # MacOS
    target_link_libraries( ${NAME}
            PRIVATE
            ${CMAKE_SOURCE_DIR}/dependency/assimp/build/bin/libassimp.5.2.4.dylib
            )
    # ... ?
endif ()
# TODO: ******************************************************************

# Link: https://coderefinery.github.io/cmake-workshop/environment/
if(CMAKE_SYSTEM_NAME STREQUAL "Linux") # Linux
    target_link_libraries( ${NAME}
            PRIVATE
            -lX11
#            -ltbb
            )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # MacOS
    target_link_libraries( ${NAME}
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
#            -ltbb
            )
    # ... ?
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Windows
    # ... ?
else()
    # ... ?
endif()

# Add custom rule (ex make fclean, re)
# ----------------------------------------------------------------------------
ADD_CUSTOM_TARGET( re
        COMMENT "Re-building..."
        COMMAND make fclean && make all
        # ...
        )

ADD_CUSTOM_TARGET( fclean
        COMMENT "Cleaning executable and binaries..."
        COMMAND make clean # cmake이 생성하는 clean은 fclean과 동일.
        )

if( DEBUG_MODE )
    print_message(WARNING "--------------------------------------------------------------------")
    print_message(WARNING "You are building project in Debug mode. (using spdlog library)")
    print_message(WARNING "If you are building release version, then set DEBUG_MODE to False in CmakeList.txt")
    print_message(WARNING "--------------------------------------------------------------------")
endif ()
