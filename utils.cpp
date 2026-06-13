// ============================================================
//  utils.cpp  –  Hàm tiện ích (ngày tháng, chuỗi, màn hình...)
// ============================================================
#include "utils.h"
#include "globals.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

// ── Hệ thống / giao diện ──────────────────────────────────────
void enableAnsi() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  m = 0;
    GetConsoleMode(h, &m);
    SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(65001);
#endif
}

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ── Xử lý chuỗi ──────────────────────────────────────────────
// Center a string within a fixed width
std::string centerStr(const std::string& s, int w) {
    int len = (int)s.size();
    if (len >= w) return s.substr(0, w);
    int l = (w - len) / 2;
    return std::string(l, ' ') + s + std::string(w - len - l, ' ');
}

// Truncate string if too long
std::string trunc(const std::string& s, int maxLen) {
    if ((int)s.size() <= maxLen) return s;
    return s.substr(0, maxLen - 3) + "...";
}

// ── Tên thứ trong tuần ────────────────────────────────────────
// Vietnamese day names (2-8)
std::string dayName(int d) {
    switch (d) {
        case 2: return "Thu Hai";   case 3: return "Thu Ba";
        case 4: return "Thu Tu";    case 5: return "Thu Nam";
        case 6: return "Thu Sau";   case 7: return "Thu Bay";
        case 8: return "Chu Nhat";  default: return "???";
    }
}
std::string dayShort(int d) {
    switch (d) {
        case 2: return "T2"; case 3: return "T3"; case 4: return "T4";
        case 5: return "T5"; case 6: return "T6"; case 7: return "T7";
        case 8: return "CN"; default: return "??";
    }
}

// ── Ngày tháng / Học kỳ ──────────────────────────────────────
// Get today's day code (2=T2 ... 8=CN)
int todayCode() {
    time_t  now = time(nullptr);
    struct tm* t = localtime(&now);
    int wday = t->tm_wday;          // 0=Sun
    return (wday == 0) ? 8 : wday + 1;
}

// Current semester string e.g. "20252"
std::string currentSemester() {
    time_t  now = time(nullptr);
    struct tm* t = localtime(&now);
    int m = t->tm_mon + 1;
    int y = t->tm_year + 1900;
    if (m >= 9)              return std::to_string(y)   + "1";
    if (m == 1)              return std::to_string(y-1) + "1";
    if (m >= 2 && m <= 6)   return std::to_string(y)   + "2";
    return std::to_string(y) + "3";  // July-August
}

// Format time_t as "dd/mm"
std::string fmtDate(time_t t) {
    struct tm* tm = localtime(&t);
    char buf[10];
    sprintf(buf, "%02d/%02d", tm->tm_mday, tm->tm_mon + 1);
    return buf;
}

// Format time_t as "dd/mm/yyyy"
std::string fmtDateFull(time_t t) {
    struct tm* tm = localtime(&t);
    char buf[12];
    sprintf(buf, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
    return buf;
}

// Monday time_t for (current week + weekOffset)
time_t weekMonday(int offset) {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    t->tm_hour = 12; t->tm_min = 0; t->tm_sec = 0;
    now = mktime(t);
    int dow = t->tm_wday;
    if (dow == 0) dow = 7;
    now -= (time_t)(dow - 1) * 86400;
    now += (time_t)offset * 7 * 86400;
    return now;
}

// ISO week number
int weekNumber(time_t t) {
    struct tm* tm = localtime(&t);
    char buf[5];
    strftime(buf, sizeof(buf), "%V", tm);
    return atoi(buf);
}

// ── Bổ trợ logic ──────────────────────────────────────────────
// Indices of classes that have a session on dayCode
std::vector<int> classesOnDay(int dayCode) {
    std::vector<int> res;
    for (int i = 0; i < (int)classes.size(); i++)
        for (auto& sc : classes[i].schedules)
            if (sc.dayOfWeek == dayCode) { res.push_back(i); break; }
    return res;
}

// Generate a unique class code 1000-9999
int genCode() {
    static std::mt19937 rng((unsigned)time(nullptr));
    std::uniform_int_distribution<int> dist(1000, 9999);
    int code;
    do { code = dist(rng); } while (usedCodes.count(code));
    usedCodes.insert(code);
    return code;
}
