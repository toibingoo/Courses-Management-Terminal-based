// ============================================================
//  database.cpp  –  Hàm đọc/ghi tệp tin (save/load/init sample)
// ============================================================
#include "database.h"
#include "globals.h"
#include "utils.h"
#include <fstream>
#include <sstream>

const std::string DATA_FILE = "data.txt";

// Schedule encoded as "day~timeSlot~room" with ';' between sessions
std::string encodeSchedules(const std::vector<Schedule>& v) {
    std::string r;
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) r += ";";
        r += std::to_string(v[i].dayOfWeek) + "~" + v[i].timeSlot + "~" + v[i].room;
    }
    return r;
}

std::vector<Schedule> decodeSchedules(const std::string& s) {
    std::vector<Schedule> v;
    std::istringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ';')) {
        if (tok.empty()) continue;
        std::istringstream ts(tok);
        std::string d, t, r;
        std::getline(ts, d, '~'); std::getline(ts, t, '~'); std::getline(ts, r, '~');
        try { v.push_back({std::stoi(d), t, r}); } catch (...) {}
    }
    return v;
}

std::string encodeInts(const std::vector<int>& v) {
    std::string r;
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) r += ";";
        r += std::to_string(v[i]);
    }
    return r;
}

std::vector<int> decodeInts(const std::string& s) {
    std::vector<int> v;
    std::istringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ';')) {
        if (!tok.empty()) try { v.push_back(std::stoi(tok)); } catch (...) {}
    }
    return v;
}

void saveData() {
    std::ofstream f(DATA_FILE);
    if (!f) return;
    f << "STUDENTS\n";
    for (auto& s : students)
        f << s.name << "|" << s.studentId << "\n";
    f << "LECTURERS\n";
    for (auto& l : lecturers)
        f << l.name << "|" << l.staffId << "|" << l.status << "\n";
    f << "CLASSES\n";
    for (auto& c : classes)
        f << c.className << "|" << c.classCode << "|" << c.semester << "|"
          << c.lecturerIndex << "|" << encodeSchedules(c.schedules) << "|"
          << encodeInts(c.studentIndices) << "\n";
}

void loadData() {
    std::ifstream f(DATA_FILE);
    if (!f) return;
    students.clear(); lecturers.clear(); classes.clear(); usedCodes.clear();
    std::string line, sec;
    while (std::getline(f, line)) {
        if (line == "STUDENTS")  { sec = "S"; continue; }
        if (line == "LECTURERS") { sec = "L"; continue; }
        if (line == "CLASSES")   { sec = "C"; continue; }
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string t;
        if (sec == "S") {
            Student st;
            std::getline(ss, st.name, '|'); std::getline(ss, st.studentId, '|');
            students.push_back(st);
        } else if (sec == "L") {
            Lecturer lc;
            std::getline(ss, lc.name, '|'); std::getline(ss, lc.staffId, '|'); std::getline(ss, lc.status, '|');
            lecturers.push_back(lc);
        } else if (sec == "C") {
            Class cl;
            std::string code, lIdx, scheds, studs;
            std::getline(ss, cl.className, '|');
            std::getline(ss, code, '|');
            std::getline(ss, cl.semester, '|');
            std::getline(ss, lIdx, '|');
            std::getline(ss, scheds, '|');
            std::getline(ss, studs);
            try { cl.classCode = std::stoi(code); } catch (...) { cl.classCode = genCode(); }
            try { cl.lecturerIndex = std::stoi(lIdx); } catch (...) { cl.lecturerIndex = 0; }
            cl.schedules      = decodeSchedules(scheds);
            cl.studentIndices = decodeInts(studs);
            usedCodes.insert(cl.classCode);
            classes.push_back(cl);
        }
    }
}

void initSample() {
    // Students (25)
    std::vector<std::pair<std::string,std::string>> sdata = {
        {"Nguyen Van An",    "20220001"}, {"Tran Thi Bich",   "20220002"},
        {"Le Van Cuong",     "20220003"}, {"Pham Thi Dung",   "20220004"},
        {"Hoang Van Em",     "20220005"}, {"Vu Thi Phuong",   "20220006"},
        {"Dang Van Giang",   "20220007"}, {"Bui Thi Ha",      "20220008"},
        {"Do Van Hieu",      "20220009"}, {"Ngo Thi Lan",     "20220010"},
        {"Dinh Van Minh",    "20220011"}, {"Truong Thi Nam",  "20220012"},
        {"Ly Van Oanh",      "20220013"}, {"Ha Thi Phuong",   "20220014"},
        {"Cao Van Quyen",    "20220015"}, {"Nguyen Thi Roi",  "20220016"},
        {"Tran Van Son",     "20220017"}, {"Le Thi Thu",      "20220018"},
        {"Pham Van Uyen",    "20220019"}, {"Hoang Thi Van",   "20220020"},
        {"Vu Van Xuan",      "20220021"}, {"Dang Thi Yen",    "20220022"},
        {"Bui Van Zung",     "20220023"}, {"Do Thi Anh",      "20220024"},
        {"Ngo Van Binh",     "20220025"}
    };
    for (auto& p : sdata) students.push_back({p.first, p.second});

    // Lecturers (3)
    lecturers.push_back({"PGS.TS. Nguyen Van Hung", "CB001", "Pho Giao su - Tien si"});
    lecturers.push_back({"TS. Tran Thi Mai",         "CB002", "Tien si"});
    lecturers.push_back({"ThS. Le Van Khanh",        "CB003", "Thac si - Giang vien chinh"});

    std::string sem = currentSemester();

    // Classes (6)
    auto addC = [&](std::string name, std::string s, int li,
                    std::vector<Schedule> sc, std::vector<int> si) {
        Class c;
        c.className    = name;  c.classCode = genCode();
        c.semester     = s;     c.lecturerIndex = li;
        c.schedules    = sc;    c.studentIndices = si;
        classes.push_back(c);
    };

    addC("Lap trinh C++",      sem, 0,
         {{2,"07:00-09:30","D3-209"},{4,"07:00-09:30","D3-209"}},
         {0,1,2,3,4,5,6,7,8,9});

    addC("Giai tich 2",        sem, 1,
         {{3,"09:45-12:15","B1-101"},{5,"09:45-12:15","B1-101"}},
         {0,2,4,6,8,10,12,14,16,18});

    addC("Vat ly dai cuong",   sem, 2,
         {{4,"13:00-15:30","C2-301"},{6,"13:00-15:30","C2-301"}},
         {1,3,5,7,9,11,13,15,17,19,20});

    addC("Co so du lieu",      sem, 0,
         {{3,"13:00-15:30","D3-105"},{6,"07:00-09:30","D3-105"}},
         {0,1,2,3,4,5,6,7,8,9,10,11});

    addC("Mang may tinh",      sem, 1,
         {{2,"13:00-15:30","D4-401"},{5,"13:00-15:30","D4-401"}},
         {10,11,12,13,14,15,16,17,18,19});

    addC("Toan roi rac",       sem, 2,
         {{7,"07:00-09:30","B1-201"}},
         {0,5,10,15,20,21,22,23,24});
}
