set(SRC_PATH $ {CMAKE_CURRENT_SOURCE_DIR})

file(GLOB MAIN_SRC $ {SRC_PATH}/*.cpp)
file(GLOB MAIN_HDR ${SRC_PATH}/*.h)
source_group("main" FILES ${MAIN_SRC} ${MAIN_HDR})

file(GLOB BASE_SRC ${SRC_PATH}/base/*.cpp)
file(GLOB BASE_HDR ${SRC_PATH}/base/*.h)
source_group("base" FILES ${BASE_SRC} ${BASE_HDR})

file(GLOB GAME_SRC ${SRC_PATH}/game/*.cpp)
file(GLOB GAME_HDR ${SRC_PATH}/game/*.h)
source_group("game" FILES ${GAME_SRC} ${GAME_HDR})

file(GLOB DATA_FILES ${CMAKE_SOURCE_DIR}/data/*.*)

set(SRC ${MAIN_SRC} ${BASE_SRC} ${GAME_SRC})
set(HDR ${MAIN_HDR} ${BASE_HDR} ${GAME_HDR})

if (APPLE)
    set_source_files_properties(${DATA_FILES} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources)
endif()

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -std=gnu++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
endif()
if(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL")
endif()
if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif()

include_directories(
    ${SRC_PATH}
    ${THIRDPARTY_PATH}/SDL2-2.0.3/include
    ${THIRDPARTY_PATH}/stb)

if (NOT APPLE)
    add_executable(GemMiner ${SRC} ${HDR})
else()
    add_executable(GemMiner MACOSX_BUNDLE ${SRC} ${HDR} ${DATA_FILES})
endif()

target_link_libraries(GemMiner SDL2 SDL2main)

if (MSCV)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
        ${CMAKE_CURRENT_BINARY_DIR}/GemMiner.vcxproj.user @ONLY)
endif()

if (NOT APPLE)
    install(TARGETS GemMiner
        RUNTIME DESTINATION bin
    )
    install(FILES ${DATA_FILES} DESTINATION bin)
endif()
