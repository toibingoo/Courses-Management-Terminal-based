// ============================================================
//  globals.h  –  Biến toàn cục & mã màu ANSI (extern declarations)
// ============================================================
#pragma once
#include <vector>
#include <set>
#include <string>
#include "types.h"

// ── Dữ liệu chính ─────────────────────────────────────────────
extern std::vector<Student>  students;
extern std::vector<Lecturer> lecturers;
extern std::vector<Class>    classes;
extern std::set<int>         usedCodes;

// ── Trạng thái điều hướng ─────────────────────────────────────
extern AppState S;

// ── Mã màu ANSI ───────────────────────────────────────────────
namespace C {
    extern const std::string R;   // Reset
    extern const std::string B;   // Bold
    extern const std::string RD;  // Red
    extern const std::string GR;  // Green
    extern const std::string YL;  // Yellow
    extern const std::string BL;  // Blue
    extern const std::string MG;  // Magenta
    extern const std::string CY;  // Cyan
    extern const std::string WH;  // White
}
