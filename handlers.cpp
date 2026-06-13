// ============================================================
//  handlers.cpp  –  Điều hướng & xử lý input
// ============================================================
#include "handlers.h"
#include "globals.h"
#include "screens.h"
#include "utils.h"
#include "database.h"
#include <iostream>
#include <string>
#include <algorithm>

void display() {
    switch (S.screen) {
        case Screen::MENU:             showMenu();            break;
        case Screen::TIMETABLE:        showTimetable();       break;
        case Screen::CLASS_LIST:       showClassList();       break;
        case Screen::CLASS_DETAIL:     showClassDetail();     break;
        case Screen::LECTURER_DETAIL:  showLecturerDetail();  break;
        case Screen::LECTURER_COURSES: showLecturerCourses(); break;
        case Screen::STUDENT_LIST:     showStudentList();     break;
    }
}

void handleInput() {
    std::string inp;
    std::getline(std::cin, inp);
    if (inp.empty()) return;

    char cmd = (char)toupper((unsigned char)inp[0]);
    int  num = 0;
    if (inp.size() > 1) { try { num = std::stoi(inp.substr(1)); } catch (...) {} }

    switch (S.screen) {

    // ------ MENU ------
    case Screen::MENU:
        if      (cmd == '1') { S.screen = Screen::TIMETABLE; S.weekOffset = 0; }
        else if (cmd == '2') { S.screen = Screen::CLASS_LIST; S.classPage = 0; }
        else if (cmd == '3') addStudentScreen();
        else if (cmd == '4') addLecturerScreen();
        else if (cmd == '5') addClassScreen();
        else if (cmd == '0') {
            saveData(); cls();
            std::cout << C::GR << "\n  Da luu du lieu. Tam biet!\n\n" << C::R;
            exit(0);
        }
        break;

    // ------ TIMETABLE ------
    case Screen::TIMETABLE:
        if (cmd == 'A') S.weekOffset--;
        else if (cmd == 'B') S.weekOffset++;
        else if (cmd == 'C' && num >= 2 && num <= 8) {
            // Show classes on a specific day this week
            cls();
            int dc = num;
            auto vec = classesOnDay(dc);
            printHeader("LOP HOC: " + dayName(dc));
            std::cout << "\n";
            if (vec.empty()) {
                std::cout << C::YL << "  Khong co lop hoc nao trong ngay nay.\n" << C::R;
            } else {
                for (int ci : vec) {
                    auto& cl = classes[ci];
                    std::cout << C::CY << "  * " << C::B << C::WH << cl.className << C::R;
                    for (auto& sc : cl.schedules)
                        if (sc.dayOfWeek == dc)
                            std::cout << C::MG << "  " << sc.timeSlot
                                 << C::YL << "  [" << sc.room << "]" << C::R;
                    std::cout << "\n";
                }
            }
            waitEnter();
        }
        else if (cmd == 'D') S.screen = Screen::MENU;
        break;

    // ------ CLASS LIST ------
    case Screen::CLASS_LIST: {
        int total = (int)classes.size();
        int pages = std::max(1, (total + 9) / 10);
        if      (cmd == 'A') { if (S.classPage + 1 < pages) S.classPage++; }
        else if (cmd == 'B') { if (S.classPage > 0) S.classPage--; }
        else if (cmd == 'C' && num >= 1 && num <= 10) {
            int abs = S.classPage * 10 + (num - 1);
            if (abs < total) { S.classIdx = abs; S.screen = Screen::CLASS_DETAIL; }
        }
        else if (cmd == 'D') S.screen = Screen::MENU;
        break;
    }

    // ------ CLASS DETAIL ------
    case Screen::CLASS_DETAIL:
        if (cmd == 'A') {
            if (S.classIdx >= 0 && S.classIdx < (int)classes.size()) {
                S.lecturerIdx = classes[S.classIdx].lecturerIndex;
                S.lecturerCoursePage = 0;
                S.screen = Screen::LECTURER_DETAIL;
            }
        }
        else if (cmd == 'B') { S.studentPage = 0; S.screen = Screen::STUDENT_LIST; }
        else if (cmd == 'C') S.screen = Screen::CLASS_LIST;
        break;

    // ------ LECTURER DETAIL ------
    case Screen::LECTURER_DETAIL:
        if      (cmd == 'A') { S.lecturerCoursePage = 0; S.screen = Screen::LECTURER_COURSES; }
        else if (cmd == 'B') S.screen = Screen::CLASS_DETAIL;
        break;

    // ------ LECTURER COURSES ------
    case Screen::LECTURER_COURSES: {
        int cnt = 0;
        for (auto& cl : classes) if (cl.lecturerIndex == S.lecturerIdx) cnt++;
        int pages = std::max(1, (cnt + 9) / 10);
        if      (cmd == 'B') { if (S.lecturerCoursePage + 1 < pages) S.lecturerCoursePage++; }
        else if (cmd == 'C') { if (S.lecturerCoursePage > 0) S.lecturerCoursePage--; }
        else if (cmd == 'D') S.screen = Screen::LECTURER_DETAIL;
        break;
    }

    // ------ STUDENT LIST ------
    case Screen::STUDENT_LIST: {
        if (S.classIdx >= 0 && S.classIdx < (int)classes.size()) {
            int total = (int)classes[S.classIdx].studentIndices.size();
            int pages = std::max(1, (total + 9) / 10);
            if      (cmd == 'A') { if (S.studentPage + 1 < pages) S.studentPage++; }
            else if (cmd == 'B') { if (S.studentPage > 0) S.studentPage--; }
        }
        if (cmd == 'C') S.screen = Screen::CLASS_DETAIL;
        break;
    }

    default: S.screen = Screen::MENU; break;
    }
}
