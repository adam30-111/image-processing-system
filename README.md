# 🖼️ CS213: Image Processing System

**Faculty of Computing and Artificial Intelligence (FCAI), Cairo University**  
**Course:** CS213 - Object-Oriented Programming (2026-2027)  
**Instructor:** Dr. Mohammad El-Ramly  

---

## 📌 Project Overview
An interactive C++ application designed to apply various image processing filters using Object-Oriented Programming (OOP) concepts and the `stb_image` single-file public domain libraries. The program provides a continuous console-based menu system that allows loading, filtering, and saving images in multiple formats (`.jpg`, `.jpeg`, `.png`, `.bmp`).

---

## 👥 Team Members & Task Distribution

According to the assignment rules, filters are distributed based on student IDs sorted in ascending order:

| Student Name | Student ID | Assigned Filters & Tasks |
|:---|:---:|:---|
| **Adam Sameh Said** | **20250075** | Filter 1 — Grayscale, Filter 5 — Flip, Filter 9 — Merge, Filter 13 — Sunlight |
| **Youssef Tamer Abdelslam** | **20250761** | Filter 2 — Black & White, Filter 6 — Rotate, Filter 10 — Detect Edges, Filter 14 — TV Screen |
| **Ibrahim Abdelrazek Mohamed** | **20251003** | Filter 3 — Invert, Filter 7 — Darken/Lighten, Filter 11 — Crop, Filter 15 — Purple Effect |
| **Fares Yasser Mahmoud** | **20251281** | Filter 4 — Frame, Filter 8 — Resize, Filter 12 — Blur, Filter 16 — Infrared |

* **Group Tasks:** Interactive Menu System, Input Validation, File Load/Save Functions, and GitHub Management.

---

## 🛠️ System Architecture & Diagram

The program follows a modular hierarchy structured as:
- **Level 0:** `main()` function running the primary menu loop.
- **Level 1:** File I/O operations (`load_image`, `save_image`) and filter selector functions.
- **Level 2:** Core mathematical algorithms for pixel manipulations per filter.

---

## 💻 Features & Supported Extensions
- **Continuous Execution:** Menu loops until the user explicitly selects `Exit`.
- **Format Verification:** Validates file existence and supported extensions (`.png`, `.jpg`, `.jpeg`, `.bmp`, `.tga`).
- **Save Flexibility:** Option to overwrite current files or save with new filenames.
- **Safety Checks:** Prevents application crashes on invalid input via error-handling bounds.

---

## 📁 Repository Directory Structure

```text
├── CS213_A1_Part1_SecX_ID1_ID2_ID3_ID4.cpp  # Main integrated source code
├── Image_Class.h                             # Given Image class wrapper
├── stb_image.h                               # STB library for loading images
├── stb_image_write.h                         # STB library for saving images
├── assets/                                   # Sample test images
├── .gitignore                                # Git ignore file for binaries
└── README.md                                 # Documentation file
