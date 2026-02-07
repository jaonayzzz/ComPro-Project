cmake_minimum_required(VERSION 3.16)
project(FlowerShop)

set(CMAKE_CXX_STANDARD 17)

# ปิด Warning
if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
endif()

# 1. ค้นหา SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# สร้างชื่อเล่นให้ SFML (ป้องกัน Error หาไม่เจอ)
if(NOT TARGET SFML::Graphics)
    add_library(SFML::Graphics INTERFACE IMPORTED)
    target_link_libraries(SFML::Graphics INTERFACE sfml-graphics sfml-window sfml-system)
    add_library(SFML::Window INTERFACE IMPORTED)
    target_link_libraries(SFML::Window INTERFACE sfml-window sfml-system)
    add_library(SFML::System INTERFACE IMPORTED)
    target_link_libraries(SFML::System INTERFACE sfml-system)
endif()

include(FetchContent)

# --- 2. ดาวน์โหลด ImGui (ใช้ชื่อ Tag v1.90 ที่ถูกต้อง) ---
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90  # <--- แก้เป็นตัวนี้ค่ะ (ตัด .0 ทิ้ง)
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

# --- 3. ดาวน์โหลด ImGui-SFML ---
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG        v2.6
)
FetchContent_GetProperties(imgui-sfml)
if(NOT imgui-sfml_POPULATED)
  FetchContent_Populate(imgui-sfml)
endif()

# --- 4. สร้างเกม ---
add_executable(MyGame 
    main.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui-sfml_SOURCE_DIR}/imgui-SFML.cpp
)

target_include_directories(MyGame PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui-sfml_SOURCE_DIR}
    ${SFML_INCLUDE_DIR} 
)

target_link_libraries(MyGame PRIVATE 
    SFML::Graphics SFML::Window SFML::System 
    opengl32
)