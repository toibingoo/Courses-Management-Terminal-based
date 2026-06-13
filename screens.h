// ============================================================
//  screens.h  –  Giao diện hiển thị các màn hình và form nhập
// ============================================================
#pragma once
#include <string>

// ── Helpers vẽ khung ──────────────────────────────────────────
void printHeader(const std::string& title, int w = 66);
void printSep(int w = 66, char ch = '-');

// ── Hàm hiển thị các màn hình ─────────────────────────────────
void showMenu();
void showTimetable();
void showClassList();
void showClassDetail();
void showLecturerDetail();
void showLecturerCourses();
void showStudentList();

// ── Các màn hình nhập liệu ────────────────────────────────────
void waitEnter();
void addStudentScreen();
void addLecturerScreen();
void addClassScreen();
