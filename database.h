// ============================================================
//  database.h  –  Hàm đọc/ghi tệp tin (save/load/init sample)
// ============================================================
#pragma once
#include <string>
#include <vector>
#include "types.h"

// ── File I/O (data.txt) ──────────────────────────────────────
extern const std::string DATA_FILE;

// ── Mã hóa/Giải mã dữ liệu ────────────────────────────────────
std::string      encodeSchedules(const std::vector<Schedule>& v);
std::vector<Schedule> decodeSchedules(const std::string& s);

std::string      encodeInts(const std::vector<int>& v);
std::vector<int> decodeInts(const std::string& s);

// ── Lưu trữ dữ liệu ───────────────────────────────────────────
void saveData();
void loadData();

// ── Dữ liệu mẫu ban đầu ───────────────────────────────────────
void initSample();
