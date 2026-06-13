// ============================================================
//  screens.cpp  –  Giao diện hiển thị các màn hình và form nhập
// ============================================================
#include "screens.h"
#include "globals.h"
#include "utils.h"
#include "database.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ── Helpers vẽ khung ──────────────────────────────────────────
void printHeader(const std::string& title, int w) {
    std::string line(w, '=');
    std::cout << C::CY << line << C::R << "\n";
    std::cout << C::CY << "  " << C::B << C::WH << centerStr(title, w-4) << C::R << "\n";
    std::cout << C::CY << line << C::R << "\n";
}

void printSep(int w, char ch) {
    std::cout << C::CY << "  " << std::string(w - 2, ch) << C::R << "\n";
}

// ── Hàm hiển thị các màn hình ─────────────────────────────────
void showMenu() {
    cls();
    time_t  now = time(nullptr);
    struct tm* t = localtime(&now);
    const char* vDays[] = {"Chu Nhat","Thu Hai","Thu Ba","Thu Tu",
                            "Thu Nam","Thu Sau","Thu Bay"};
    char dateBuf[64];
    sprintf(dateBuf, "%s, %02d/%02d/%04d",
            vDays[t->tm_wday], t->tm_mday, t->tm_mon+1, t->tm_year+1900);

    std::cout << "\n";
    printHeader("UNIVERSITY SCHEDULE MANAGER");
    std::cout << "\n";
    std::cout << "  " << C::B << C::WH << dateBuf << C::R
         << C::CY << "  |  " << C::R
         << C::GR << C::B << "Hoc ky: " << currentSemester() << C::R << "\n\n";

    // Today's classes
    int dc = todayCode();
    auto tc = classesOnDay(dc);
    if (tc.empty()) {
        std::cout << C::YL << "  Hom nay ban khong co lop hoc nao.\n" << C::R;
    } else {
        std::cout << C::GR << C::B << "  Cac lop hoc hom nay:\n" << C::R;
        for (int ci : tc) {
            auto& cl = classes[ci];
            for (auto& sc : cl.schedules)
                if (sc.dayOfWeek == dc)
                    std::cout << C::CY << "    > " << C::R
                         << C::WH << std::left << std::setw(26) << trunc(cl.className,24) << C::R
                         << C::MG << std::setw(16) << sc.timeSlot << C::R
                         << C::YL << "[" << sc.room << "]" << C::R << "\n";
        }
    }

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::B << C::WH << "[1]" << C::R << " Time Table\n";
    std::cout << "  " << C::B << C::WH << "[2]" << C::R << " Danh sach cac lop\n";
    std::cout << "  " << C::B << C::WH << "[3]" << C::R << " Them sinh vien moi\n";
    std::cout << "  " << C::B << C::WH << "[4]" << C::R << " Them giang vien moi\n";
    std::cout << "  " << C::B << C::WH << "[5]" << C::R << " Them lop hoc moi\n";
    std::cout << "  " << C::B << C::WH << "[0]" << C::R << " Luu & Thoat\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showTimetable() {
    cls();
    time_t  mon  = weekMonday(S.weekOffset);
    int     wNum = weekNumber(mon);

    // Build day times
    time_t days[7];
    for (int i = 0; i < 7; i++) days[i] = mon + (time_t)i * 86400;

    std::string title = "TUAN " + std::to_string(wNum)
                 + " (" + fmtDate(days[0]) + " - " + fmtDate(days[6]) + ")";
    if (S.weekOffset == 0) title += " [TUAN NAY]";
    else if (S.weekOffset < 0) title += " [" + std::to_string(-S.weekOffset) + " TUAN TRUOC]";
    else title += " [" + std::to_string(S.weekOffset) + " TUAN SAU]";

    std::cout << "\n";
    printHeader(title);
    std::cout << "\n";

    // Column header
    std::cout << C::CY
         << std::left << std::setw(7) << "  NGAY"
         << std::setw(8) << "DATE"
         << "LOP HOC\n";
    printSep(66, '-');

    int today = todayCode();
    for (int i = 0; i < 7; i++) {
        int  dc  = i + 2;           // 2=T2 ... 8=CN
        auto vec = classesOnDay(dc);
        bool isT = (S.weekOffset == 0 && dc == today);

        struct tm* dTm = localtime(&days[i]);
        char dateBuf[8];
        sprintf(dateBuf, "%02d/%02d", dTm->tm_mday, dTm->tm_mon+1);

        std::string dayLabel = dayShort(dc) + (isT ? "*" : " ");
        if (isT) std::cout << C::GR << C::B;
        else     std::cout << C::WH;

        std::cout << "  " << std::setw(5) << dayLabel << std::setw(8) << dateBuf;

        if (vec.empty()) {
            std::cout << C::YL << "(khong co lop)" << C::R << "\n";
        } else {
            bool first = true;
            for (int ci : vec) {
                if (!first) std::cout << std::string(15, ' ');
                auto& cl = classes[ci];
                for (auto& sc : cl.schedules) {
                    if (sc.dayOfWeek == dc) {
                        std::cout << C::R << std::left << std::setw(24) << trunc(cl.className, 22)
                             << C::MG << std::setw(16) << sc.timeSlot
                             << C::YL << "[" << sc.room << "]" << C::R;
                    }
                }
                std::cout << "\n";
                first = false;
            }
        }
        if (i < 6) std::cout << C::CY << "  " << std::string(62,'.') << C::R << "\n";
    }

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[A]" << C::R << " Tuan truoc  "
         << C::WH << "[B]" << C::R << " Tuan sau  "
         << C::WH << "[C2-C8]" << C::R << " Xem ngay  "
         << C::WH << "[D]" << C::R << " Menu\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showClassList() {
    cls();
    int total  = (int)classes.size();
    int pages  = std::max(1, (total + 9) / 10);
    S.classPage = std::max(0, std::min(S.classPage, pages - 1));
    int st = S.classPage * 10, en = std::min(st + 10, total);

    std::cout << "\n";
    printHeader("DANH SACH CAC LOP HOC");
    std::cout << C::CY << "  Trang " << C::B << (S.classPage+1) << "/" << pages
         << C::R << C::WH << "  (Tong: " << total << " lop)" << C::R << "\n\n";

    std::cout << C::CY << "  " << std::left << std::setw(5) << "STT"
         << std::setw(28) << "TEN LOP" << std::setw(9) << "HOC KY"
         << std::setw(8) << "MA LOP" << "GIANG VIEN\n";
    printSep(66, '-');

    if (total == 0) {
        std::cout << C::YL << "  (Chua co lop hoc nao)\n" << C::R;
    } else {
        for (int i = st; i < en; i++) {
            auto& cl = classes[i];
            int   n  = i - st + 1;
            std::string lname = (cl.lecturerIndex >= 0 &&
                            cl.lecturerIndex < (int)lecturers.size())
                           ? trunc(lecturers[cl.lecturerIndex].name, 22) : "-";
            std::cout << "  " << C::WH << std::setw(5) << n << C::R
                 << std::setw(28) << trunc(cl.className, 26)
                 << C::CY << std::setw(9) << cl.semester
                 << C::YL << std::setw(8) << cl.classCode
                 << C::GR << lname << C::R << "\n";
        }
    }

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[A]" << C::R << " Trang sau  "
         << C::WH << "[B]" << C::R << " Trang truoc  "
         << C::WH << "[C+so]" << C::R << " Chi tiet (1-10)  "
         << C::WH << "[D]" << C::R << " Menu\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showClassDetail() {
    cls();
    if (S.classIdx < 0 || S.classIdx >= (int)classes.size()) {
        S.screen = Screen::CLASS_LIST; return;
    }
    auto& cl = classes[S.classIdx];

    std::cout << "\n";
    printHeader("CHI TIET LOP HOC");
    std::cout << "\n";
    std::cout << C::CY << "  Ten lop    : " << C::B << C::WH << cl.className << C::R << "\n";
    std::cout << C::CY << "  Ma lop     : " << C::YL << cl.classCode << C::R << "\n";
    std::cout << C::CY << "  Hoc ky     : " << C::WH << cl.semester << C::R << "\n";

    if (cl.lecturerIndex >= 0 && cl.lecturerIndex < (int)lecturers.size())
        std::cout << C::CY << "  Giang vien : " << C::GR
             << lecturers[cl.lecturerIndex].name << C::R << "\n";

    std::cout << "\n" << C::CY << "  Lich hoc (cac buoi co dinh trong tuan):\n" << C::R;
    if (cl.schedules.empty()) {
        std::cout << "    " << C::YL << "(Chua co lich hoc)\n" << C::R;
    } else {
        for (auto& sc : cl.schedules)
            std::cout << "    " << C::MG << "* " << C::R
                 << std::left << std::setw(12) << dayName(sc.dayOfWeek)
                 << C::YL << std::setw(16) << sc.timeSlot
                 << C::WH << "[" << sc.room << "]" << C::R << "\n";
    }

    std::cout << C::CY << "\n  So sinh vien: " << C::WH
         << cl.studentIndices.size() << C::R << "\n";

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[A]" << C::R << " Chi tiet GV  "
         << C::WH << "[B]" << C::R << " Danh sach SV  "
         << C::WH << "[C]" << C::R << " Ve danh sach lop\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showLecturerDetail() {
    cls();
    if (S.lecturerIdx < 0 || S.lecturerIdx >= (int)lecturers.size()) {
        S.screen = Screen::CLASS_DETAIL; return;
    }
    auto& l = lecturers[S.lecturerIdx];
    int cnt = 0;
    for (auto& cl : classes) if (cl.lecturerIndex == S.lecturerIdx) cnt++;

    std::cout << "\n";
    printHeader("CHI TIET GIANG VIEN");
    std::cout << "\n";
    std::cout << C::CY << "  Ten          : " << C::B << C::WH << l.name   << C::R << "\n";
    std::cout << C::CY << "  Ma can bo    : " << C::YL << l.staffId         << C::R << "\n";
    std::cout << C::CY << "  Trang thai   : " << C::GR << l.status          << C::R << "\n";
    std::cout << C::CY << "  So lop phu trach: " << C::WH << cnt            << C::R << "\n";

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[A]" << C::R << " Ds mon dam nhan  "
         << C::WH << "[B]" << C::R << " Ve chi tiet lop\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showLecturerCourses() {
    cls();
    if (S.lecturerIdx < 0) { S.screen = Screen::LECTURER_DETAIL; return; }

    std::vector<int> lc;
    for (int i = 0; i < (int)classes.size(); i++)
        if (classes[i].lecturerIndex == S.lecturerIdx) lc.push_back(i);

    int total = (int)lc.size();
    int pages = std::max(1, (total + 9) / 10);
    S.lecturerCoursePage = std::max(0, std::min(S.lecturerCoursePage, pages - 1));
    int st = S.lecturerCoursePage * 10, en = std::min(st + 10, total);

    std::string title = "MON CUA: " + trunc(lecturers[S.lecturerIdx].name, 40);
    std::cout << "\n";
    printHeader(title);
    std::cout << C::CY << "  Trang " << C::B << (S.lecturerCoursePage+1) << "/" << pages << C::R << "\n\n";

    if (total == 0) {
        std::cout << C::YL << "  (Giang vien nay chua phu trach lop nao)\n" << C::R;
    } else {
        std::cout << C::CY << "  " << std::setw(5) << "STT" << std::setw(30) << "TEN LOP" << "HOC KY\n";
        printSep(50,'-');
        for (int i = st; i < en; i++) {
            auto& cl = classes[lc[i]];
            std::cout << "  " << C::WH << std::setw(5) << (i-st+1) << C::R
                 << std::setw(30) << trunc(cl.className, 28)
                 << C::CY << cl.semester << C::R << "\n";
        }
    }

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[B]" << C::R << " Trang sau  "
         << C::WH << "[C]" << C::R << " Trang truoc  "
         << C::WH << "[D]" << C::R << " Ve chi tiet GV\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

void showStudentList() {
    cls();
    if (S.classIdx < 0 || S.classIdx >= (int)classes.size()) {
        S.screen = Screen::CLASS_DETAIL; return;
    }
    auto& cl    = classes[S.classIdx];
    int   total = (int)cl.studentIndices.size();
    int   pages = std::max(1, (total + 9) / 10);
    S.studentPage = std::max(0, std::min(S.studentPage, pages - 1));
    int st = S.studentPage * 10, en = std::min(st + 10, total);

    std::cout << "\n";
    printHeader("DANH SACH SV: " + trunc(cl.className, 40));
    std::cout << C::CY << "  Trang " << C::B << (S.studentPage+1) << "/" << pages
         << C::R << C::WH << "  (Tong: " << total << " SV)" << C::R << "\n\n";

    std::cout << C::CY << "  " << std::left << std::setw(5) << "STT"
         << std::setw(26) << "HO TEN" << "MSSV\n";
    printSep(45, '-');

    if (total == 0) {
        std::cout << C::YL << "  (Chua co sinh vien nao trong lop)\n" << C::R;
    } else {
        for (int i = st; i < en; i++) {
            int si = cl.studentIndices[i];
            if (si < 0 || si >= (int)students.size()) continue;
            auto& sv = students[si];
            std::cout << "  " << C::WH << std::setw(5) << (i-st+1) << C::R
                 << std::setw(26) << trunc(sv.name, 24)
                 << C::YL << sv.studentId << C::R << "\n";
        }
    }

    std::cout << "\n";
    printSep();
    std::cout << "  " << C::WH << "[A]" << C::R << " Trang sau  "
         << C::WH << "[B]" << C::R << " Trang truoc  "
         << C::WH << "[C]" << C::R << " Ve chi tiet lop\n";
    printSep();
    std::cout << C::YL << "  Chon: " << C::R;
}

// ── Các màn hình nhập liệu ────────────────────────────────────
void waitEnter() {
    std::cout << C::CY << "\n  Nhan Enter de tiep tuc..." << C::R;
    std::cin.get();
}

void addStudentScreen() {
    cls();
    printHeader("THEM SINH VIEN MOI");
    std::cout << "\n";

    Student sv;
    std::cout << C::CY << "  Ho ten    : " << C::R;
    std::getline(std::cin, sv.name);
    if (sv.name.empty()) return;

    std::cout << C::CY << "  Ma so SV  : " << C::R;
    std::getline(std::cin, sv.studentId);

    for (auto& ex : students) {
        if (ex.studentId == sv.studentId) {
            std::cout << C::RD << "\n  [!] Ma so SV nay da ton tai!\n" << C::R;
            waitEnter(); return;
        }
    }

    students.push_back(sv);
    saveData();
    std::cout << C::GR << "\n  [OK] Da them: " << sv.name
         << " (" << sv.studentId << ")\n" << C::R;
    waitEnter();
}

void addLecturerScreen() {
    cls();
    printHeader("THEM GIANG VIEN MOI");
    std::cout << "\n";

    Lecturer l;
    std::cout << C::CY << "  Ho ten       : " << C::R; std::getline(std::cin, l.name);
    if (l.name.empty()) return;

    std::cout << C::CY << "  Ma can bo     : " << C::R; std::getline(std::cin, l.staffId);
    std::cout << C::CY << "  Trang thai    : " << C::R; std::getline(std::cin, l.status);

    for (auto& ex : lecturers) {
        if (ex.staffId == l.staffId) {
            std::cout << C::RD << "\n  [!] Ma can bo nay da ton tai!\n" << C::R;
            waitEnter(); return;
        }
    }

    lecturers.push_back(l);
    saveData();
    std::cout << C::GR << "\n  [OK] Da them GV: " << l.name << "\n" << C::R;
    waitEnter();
}

void addClassScreen() {
    cls();
    printHeader("THEM LOP HOC MOI");
    std::cout << "\n";

    if (lecturers.empty()) {
        std::cout << C::RD << "  [!] Chua co giang vien nao. Vui long them GV truoc!\n" << C::R;
        waitEnter(); return;
    }

    Class cl;
    std::cout << C::CY << "  Ten lop          : " << C::R; std::getline(std::cin, cl.className);
    if (cl.className.empty()) return;

    std::cout << C::CY << "  Hoc ky (vd 20252): " << C::R; std::getline(std::cin, cl.semester);

    // Choose lecturer
    std::cout << "\n" << C::CY << "  Danh sach giang vien:\n" << C::R;
    for (int i = 0; i < (int)lecturers.size(); i++)
        std::cout << "    " << C::WH << (i+1) << ". " << C::R << lecturers[i].name << "\n";
    std::cout << C::CY << "  Chon GV (1-" << lecturers.size() << "): " << C::R;
    std::string s; std::getline(std::cin, s);
    int li = 0;
    try { li = std::stoi(s) - 1; } catch (...) {}
    cl.lecturerIndex = std::max(0, std::min((int)lecturers.size()-1, li));

    // Add schedules
    std::cout << "\n" << C::CY << "  Them buoi hoc (ngay: 2=T2...8=CN, 0=xong):\n" << C::R;
    while (true) {
        std::cout << C::CY << "  Ngay (0=xong): " << C::R;
        std::string ds; std::getline(std::cin, ds);
        int day = 0;
        try { day = std::stoi(ds); } catch (...) {}
        if (day == 0) break;
        if (day < 2 || day > 8) {
            std::cout << C::RD << "  Ngay khong hop le (2-8)!\n" << C::R; continue;
        }
        Schedule sc; sc.dayOfWeek = day;
        std::cout << C::CY << "  Gio (vd 07:00-09:30): " << C::R; std::getline(std::cin, sc.timeSlot);
        std::cout << C::CY << "  Phong (vd D3-209)   : " << C::R; std::getline(std::cin, sc.room);
        cl.schedules.push_back(sc);
        std::cout << C::GR << "  [+] " << dayName(day) << " " << sc.timeSlot << "\n" << C::R;
    }

    // Optionally add existing students
    std::cout << "\n" << C::CY << "  Them sinh vien vao lop? (c/n): " << C::R;
    std::string yn; std::getline(std::cin, yn);
    if (!yn.empty() && (yn[0]=='c'||yn[0]=='C'||yn[0]=='y'||yn[0]=='Y')) {
        std::cout << C::CY << "  Nhap MSSV (Enter tung ma, nhap 'xong' de thoat):\n" << C::R;
        while (true) {
            std::cout << C::CY << "  MSSV: " << C::R;
            std::string id; std::getline(std::cin, id);
            if (id == "xong" || id.empty()) break;
            bool found = false;
            for (int i = 0; i < (int)students.size(); i++) {
                if (students[i].studentId == id) {
                    // Avoid duplicate in class
                    if (std::find(cl.studentIndices.begin(),
                             cl.studentIndices.end(), i) == cl.studentIndices.end()) {
                        cl.studentIndices.push_back(i);
                        std::cout << C::GR << "  [+] " << students[i].name << "\n" << C::R;
                    } else {
                        std::cout << C::YL << "  Da co trong lop.\n" << C::R;
                    }
                    found = true; break;
                }
            }
            if (!found) std::cout << C::RD << "  Khong tim thay MSSV nay.\n" << C::R;
        }
    }

    cl.classCode = genCode();
    classes.push_back(cl);
    saveData();
    std::cout << C::GR << "\n  [OK] Da them lop: " << cl.className
         << "  Ma: " << cl.classCode << "\n" << C::R;
    waitEnter();
}
