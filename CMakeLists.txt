cmake_minimum_required(VERSION 3.20)
project(MyProgram)

# บังคับใช้ Policy ใหม่เพื่อแก้ปัญหา freetype ที่คุณเจอ
set(CMAKE_POLICY_DEFAULT_CMP0000 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. โหลด SFML 3 (ตัวหลักสำหรับกราฟิก)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. โหลด Dear ImGui (สำหรับสร้างหน้าจอเมนู/ปุ่ม)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.1
)
FetchContent_MakeAvailable(imgui)

# 3. โหลด ImGui-SFML (ตัวเชื่อมที่สำคัญ)
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        v3.0
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_MakeAvailable(imgui-sfml)

# 4. ระบุชื่อไฟล์หลักของโปรแกรม
add_executable(MyProgram examplemain.cpp)

# 5. เชื่อมต่อ Library ทุกตัวเข้าด้วยกัน
target_link_libraries(MyProgram 
    PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    ImGui-SFML::ImGui-SFML
)