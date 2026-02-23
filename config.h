#pragma once
/*
=====================================================================
 config.h
 --------------------------------------------------------------------
 ไฟล์นี้ใช้เก็บ:
 - ค่าพื้นฐานของหน้าต่าง
 - โทนสีหลักของเกม
 - สีของ ImGui UI ทุกส่วน
 - ความโค้งของ UI

 หลักการใช้งาน
 - ห้ามเขียน Logic เกมในไฟล์นี้
 - ถ้าจะเปลี่ยนสี แก้ที่นี่ที่เดียว
=====================================================================
*/

#include <SFML/Graphics.hpp>
#include <imgui.h>



// ======================================================
// Window Configuration
// ======================================================

// ความกว้างของหน้าต่าง (หน่วย: pixel)
const unsigned int WINDOW_WIDTH  = 1280;

// ความสูงของหน้าต่าง (หน่วย: pixel)
const unsigned int WINDOW_HEIGHT = 720;

// จำกัดเฟรมเรต เพื่อไม่ให้กินทรัพยากรเครื่องมากเกินไป
const unsigned int TARGET_FPS    = 60;

// ชื่อเกมที่แสดงบน Title Bar ของหน้าต่าง
const char WINDOW_TITLE[] = "Flower Shop Game";



// ======================================================
// Background Gradient (SFML)
// ------------------------------------------------------
// สีพื้นหลังแบบไล่เฉด (Gradient)
// ใช้กับ sf::VertexArray เพื่อวาดฉากหลัง
// ======================================================

// มุมซ้ายบน ชมพูพาสเทล
const sf::Color BG_GRADIENT_TOP_LEFT =
    sf::Color(255, 230, 240);

// มุมขวาบน ครีมอมเหลือง
const sf::Color BG_GRADIENT_TOP_RIGHT =
    sf::Color(255, 245, 220);

// มุมซ้ายล่าง ฟ้าอ่อน
const sf::Color BG_GRADIENT_BOTTOM_LEFT =
    sf::Color(230, 240, 255);

// มุมขวาล่าง ม่วงอ่อน
const sf::Color BG_GRADIENT_BOTTOM_RIGHT =
    sf::Color(245, 230, 255);

/*
 การทำงาน:
 - SFML จะผสมสีจากทั้ง 4 มุมอัตโนมัติ
*/



// ======================================================
// ImGui Theme Colors
// ------------------------------------------------------
// สีทั้งหมดของ UI (Text, Button, Combo, Popup)
// ค่า ImVec4 = (Red, Green, Blue, Alpha)
// ค่าแต่ละช่องอยู่ในช่วง 0.0 - 1.0
// ======================================================


// ----------------------
// Text Color
// ----------------------
// สีตัวอักษรหลักของ UI
const ImVec4 COLOR_TEXT =
    ImVec4(0.30f, 0.25f, 0.35f, 1.00f);


// ----------------------
// Window Background
// ----------------------
// พื้นหลังหน้าต่าง ImGui
// ขาว
const ImVec4 COLOR_WINDOW_BG =
    ImVec4(1.00f, 1.00f, 1.00f, 0.95f);


// ----------------------
// Button Colors
// ----------------------
// สีปุ่มปกติ ชมพูพาสเทล
const ImVec4 COLOR_BUTTON =
    ImVec4(1.00f, 0.75f, 0.85f, 1.00f);

// สีปุ่มตอนเอาเมาส์ไปชี้ เข้มขึ้น
const ImVec4 COLOR_BUTTON_HOVER =
    ImVec4(1.00f, 0.65f, 0.80f, 1.00f);

// สีปุ่มตอนกด เข้มที่สุด
const ImVec4 COLOR_BUTTON_ACTIVE =
    ImVec4(1.00f, 0.55f, 0.75f, 1.00f);


// ----------------------
// Frame / Combo / Input
// ----------------------
// ใช้กับ ComboBox, InputText, Dropdown
const ImVec4 COLOR_FRAME_BG =
    ImVec4(1.00f, 0.92f, 0.96f, 1.00f);

const ImVec4 COLOR_FRAME_BG_HOVER =
    ImVec4(1.00f, 0.88f, 0.94f, 1.00f);

const ImVec4 COLOR_FRAME_BG_ACTIVE =
    ImVec4(1.00f, 0.85f, 0.92f, 1.00f);


// ----------------------
// Popup / Dropdown Background
// ----------------------
// พื้นหลังเมนูที่เด้งออกมา
const ImVec4 COLOR_POPUP_BG =
    ImVec4(1.00f, 0.97f, 0.98f, 0.98f);



// ======================================================
// ความโค้ง
// ------------------------------------------------------
// ค่ายิ่งมาก มุมมน 
// ค่า 0.0f เหลี่ยม
// ======================================================

// ความโค้งของหน้าต่างหลัก
const float ROUND_WINDOW = 16.f;

// ความโค้งของปุ่ม / combo / input
const float ROUND_FRAME  = 12.f;

// ความโค้งของ popup / dropdown
const float ROUND_POPUP  = 12.f;