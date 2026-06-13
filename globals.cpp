// ============================================================
//  globals.cpp  –  Khởi tạo biến toàn cục & mã màu ANSI
// ============================================================
#include "globals.h"

// ── Dữ liệu chính ─────────────────────────────────────────────
std::vector<Student>  students;
std::vector<Lecturer> lecturers;
std::vector<Class>    classes;
std::set<int>         usedCodes;

// ── Trạng thái điều hướng ─────────────────────────────────────
AppState S;

// ── Mã màu ANSI ───────────────────────────────────────────────
namespace C {
    const std::string R  = "\033[0m";
    const std::string B  = "\033[1m";
    const std::string RD = "\033[91m";
    const std::string GR = "\033[92m";
    const std::string YL = "\033[93m";
    const std::string BL = "\033[94m";
    const std::string MG = "\033[95m";
    const std::string CY = "\033[96m";
    const std::string WH = "\033[97m";
}
