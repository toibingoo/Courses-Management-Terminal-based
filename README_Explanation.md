# Giải thích chuyên sâu về Cấu trúc Chương trình và OOP

Tài liệu này giải thích chi tiết cấu trúc kiến trúc của chương trình quản lý lịch học (được mô tả trong `architecture.mmd`) và cấu trúc hướng đối tượng (OOP) kèm kế thừa (mô tả trong `oop_structure.mmd`). Việc hiểu rõ hai cấu trúc này là chìa khóa để nắm bắt toàn bộ cách hệ thống hoạt động.

---

## 1. Cấu trúc Chương trình (Architecture)

Chương trình được chia thành nhiều module (tệp) khác nhau dựa trên nguyên tắc **Separation of Concerns (Phân tách mối quan tâm)**. Thay vì nhồi nhét tất cả vào một file, mỗi module sẽ đảm nhận một nhiệm vụ cụ thể:

### A. Vòng đời và Luồng điều khiển chính (`main.cpp` & `handlers.cpp`)
* **`main.cpp`**: Là điểm bắt đầu của ứng dụng. Nhiệm vụ duy nhất của nó là khởi tạo môi trường (màu ANSI), tải dữ liệu từ tệp (`loadData()`), và chạy một **vòng lặp vô hạn (infinite loop)**.
* **Vòng lặp chính**: Liên tục gọi hai hàm:
  1. `display()`: Dựa vào trạng thái hiện tại (nằm ở màn hình nào) để vẽ giao diện tương ứng.
  2. `handleInput()`: Đợi người dùng nhập lệnh, sau đó cập nhật trạng thái (state) hoặc gọi các hàm xử lý. Đây chính là **Bộ điều khiển (Controller)** của chương trình.

### B. Giao diện người dùng (`screens.cpp`)
* Module này (đóng vai trò **View**) chỉ chứa các hàm chuyên dùng để in ra màn hình (`cout`). 
* Khi ở màn hình Menu, `showMenu()` được gọi. Khi xem chi tiết lớp, `showClassDetail()` được gọi. Module này hoàn toàn tách biệt khỏi logic xử lý input, giúp giao diện dễ dàng thay đổi mà không làm hỏng logic chạy bên trong.

### C. Xử lý Dữ liệu và Tiện ích (`database.cpp` & `utils.cpp`)
* **`database.cpp`**: (Đóng vai trò **Model**). Chịu trách nhiệm chuyển đổi dữ liệu trên RAM (các vector) thành văn bản để lưu vào tệp `data.txt` (`saveData`), và ngược lại (`loadData`).
* **`utils.cpp`**: Chứa các hàm tính toán dùng chung như tính toán tuần, thứ, căn giữa chuỗi. Điều này giúp mã nguồn ở các nơi khác không bị lặp lại (DRY - Don't Repeat Yourself).

### D. Trạng thái toàn cục (`globals.cpp` & `types.h`)
* Chương trình sử dụng một biến trạng thái `AppState S` (lưu màn hình hiện tại, trang hiện tại, sinh viên đang chọn).
* Dữ liệu các thực thể (`students`, `lecturers`, `classes`) được thiết kế ở dạng **Global State** (Trạng thái toàn cục), cho phép tất cả các file (screens, database, handlers) đều có thể đọc/ghi một cách dễ dàng.

---

## 2. Cấu trúc OOP và Kế thừa (OOP & Inheritance)

Trong quá trình phát triển mã nguồn C++, dù ban đầu bạn có thể sử dụng các `struct` đơn giản để lưu trữ dữ liệu, nhưng khi chương trình lớn lên, thiết kế theo mô hình **Hướng Đối Tượng (OOP)** với tính năng **Kế thừa (Inheritance)** là bắt buộc. Sơ đồ `oop_structure.mmd` đã vẽ ra một bản đồ thiết kế nâng cao:

### A. Lớp cơ sở Abstract (`Person`)
Cả Sinh viên (`Student`) và Giảng viên (`Lecturer`) đều là con người. Họ đều có **Tên**.
* Chúng ta tạo một lớp cha là `Person` có thuộc tính `name` (được bảo vệ - `protected`).
* `Person` sẽ có một phương thức ảo (Virtual Method) là `displayInfo()`. Việc để hàm này ảo (hoặc thuần ảo `virtual void displayInfo() = 0`) giúp nó trở thành một interface bắt buộc các lớp con phải tự triển khai.

### B. Kế thừa (Inheritance) ở `Student` và `Lecturer`
* **Sinh viên (`Student`)** kế thừa từ `Person`. Thuộc tính riêng của sinh viên là mã số sinh viên (`studentId`). Khi ghi đè (`override`) hàm `displayInfo()`, nó sẽ in ra: Tên (từ lớp cha) + MSSV.
* **Giảng viên (`Lecturer`)** kế thừa từ `Person`. Thuộc tính riêng là mã cán bộ (`staffId`) và học hàm/học vị (`status`). Hàm `displayInfo()` sẽ in ra: Tên (lớp cha) + Mã CB + Học vị.
* **Lợi ích:** Tránh việc phải khai báo lại biến `name` ở khắp nơi. Giúp dễ dàng tạo ra một danh sách chứa chung cả Giảng viên và Sinh viên (bằng cách dùng con trỏ lớp cha `vector<Person*>`).

### C. Tính Bao Đóng (Encapsulation)
Trong sơ đồ OOP, các thuộc tính như `classCode`, `semester` trong lớp `Class` được để ở chế độ `private` (dấu `-`).
* Điều này ngăn chặn việc các module khác gán sai dữ liệu (ví dụ: vô tình đổi mã lớp).
* Để truy cập, ta phải dùng các hàm getter/setter công khai (ví dụ `addClass()`, `addStudent()`). Điều này đảm bảo tính toàn vẹn dữ liệu.

### D. Các Mối Quan Hệ (Relationships)
Sơ đồ OOP cũng cho thấy các kết nối cơ bản:
* **Composition (Bao gồm):** Một `Class` (Lớp học) **chứa** nhiều `Schedule` (Lịch học). Nếu lớp học bị xóa, lịch học đó cũng biến mất. Đây là quan hệ chặt chẽ.
* **Association (Liên kết):** `Lecturer` (Giảng viên) có liên kết với `Class` (Phụ trách), và `Student` (Sinh viên) liên kết với `Class` (Tham gia). Sự tồn tại của Giảng viên độc lập với Lớp học. Nếu Lớp học bị hủy, Giảng viên vẫn tồn tại.

---

### Tổng kết
Sự kết hợp giữa **Kiến trúc Module (phân tách file rõ ràng)** và **Tư duy OOP (Gói gọn dữ liệu, Kế thừa đặc tính)** tạo ra một hệ thống phần mềm chuyên nghiệp. Khi đọc mã nguồn, bạn hãy nhìn nó như một cỗ máy:
1. `types.h` là các "khuôn đúc" linh kiện.
2. `database.cpp` là nhà kho chứa dữ liệu.
3. `screens.cpp` là lớp sơn và bảng đèn LED bên ngoài.
4. `handlers.cpp` là bộ não và bảng điều khiển.
5. Vòng lặp `main.cpp` chính là động cơ giữ cho cỗ máy không ngừng chạy.
