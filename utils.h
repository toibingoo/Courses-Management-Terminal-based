// ============================================================
//  utils.h  –  Hàm tiện ích (ngày tháng, chuỗi, màn hình...)
// ============================================================
#pragma once
#include <string>
#include <vector>
#include "types.h"

// ── Hệ thống / giao diện ──────────────────────────────────────
void enableAnsi();
void cls();

// ── Xử lý chuỗi ──────────────────────────────────────────────
std::string centerStr(const std::string& s, int w);
std::string trunc(const std::string& s, int maxLen);

// ── Tên thứ trong tuần ────────────────────────────────────────
std::string dayName(int d);   // "Thu Hai" ... "Chu Nhat"
std::string dayShort(int d);  // "T2" ... "CN"

// ── Ngày tháng / Học kỳ ──────────────────────────────────────
int         todayCode();                   // 2=T2 ... 8=CN
std::string currentSemester();             // e.g. "20252"
std::string fmtDate(time_t t);             // "dd/mm"
std::string fmtDateFull(time_t t);         // "dd/mm/yyyy"
time_t      weekMonday(int offset);        // Monday of (current+offset) week
int         weekNumber(time_t t);          // ISO week number

// ── Bổ trợ logic ──────────────────────────────────────────────
std::vector<int> classesOnDay(int dayCode);   // indices of classes on day
int              genCode();                    // unique class code 1000-9999
