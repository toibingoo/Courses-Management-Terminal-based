// ============================================================
//  main.cpp  –  Điểm khởi đầu và vòng lặp chính
// ============================================================
#include "types.h"
#include "globals.h"
#include "utils.h"
#include "database.h"
#include "handlers.h"

int main() {
    enableAnsi();

    // Load from file; if empty, load sample data
    loadData();
    if (classes.empty() && students.empty() && lecturers.empty()) {
        initSample();
        saveData();
    }

    // Main loop
    while (true) {
        display();
        handleInput();
    }

    return 0;
}
