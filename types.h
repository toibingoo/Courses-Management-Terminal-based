// ============================================================
//  types.h  –  Structs, enum Screen, AppState
// ============================================================
#pragma once
#include <string>
#include <vector>

// ── Data structs ─────────────────────────────────────────────
struct Schedule {
    int    dayOfWeek;   // 2=T2 … 8=CN
    std::string timeSlot;    // e.g. "07:00-09:30"
    std::string room;        // e.g. "D3-209"
};

struct Student {
    std::string name;
    std::string studentId;
};

struct Lecturer {
    std::string name;
    std::string staffId;
    std::string status;
};

struct Class {
    std::string          className;
    int                  classCode;       // unique 1000-9999
    std::string          semester;
    int                  lecturerIndex;   // index into lecturers[]
    std::vector<Schedule> schedules;
    std::vector<int>     studentIndices;  // indices into students[]
};

// ── Navigation state machine ──────────────────────────────────
enum class Screen {
    MENU, TIMETABLE,
    CLASS_LIST, CLASS_DETAIL,
    LECTURER_DETAIL, LECTURER_COURSES,
    STUDENT_LIST
};

struct AppState {
    Screen screen             = Screen::MENU;
    int    weekOffset         = 0;   // 0 = this week
    int    classPage          = 0;
    int    classIdx           = -1;
    int    lecturerIdx        = -1;
    int    lecturerCoursePage = 0;
    int    studentPage        = 0;
};
