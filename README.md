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

graph TD

    classDef default font-family:sans-serif,font-size:14px,color:#000;
    classDef mainBox fill:#ffffff,stroke:#000000,stroke-width:3px,font-weight:bold,font-size:18px;
    classDef level1 fill:#f9f9f9,stroke:#555555,stroke-width:2px,font-weight:bold;
    
    classDef adam fill:#E6E6FA,stroke:#4B0082,stroke-width:2px,font-weight:bold;
    classDef youssef fill:#E0FFF0,stroke:#006400,stroke-width:2px,font-weight:bold;
    classDef ibrahim fill:#FFEFD5,stroke:#B85C00,stroke-width:2px,font-weight:bold;
    classDef fares fill:#FFE4E1,stroke:#8B0000,stroke-width:2px,font-weight:bold;
    classDef team fill:#E8E8E8,stroke:#333333,stroke-width:2px,font-weight:bold;

    Main["main()<br><span style='font-size:12px; font-weight:normal;'>window + event loop</span>"]:::mainBox

    Open["Open<br><span style='font-size:12px; font-weight:normal;'>onLoadClicked</span>"]:::level1
    Save["Save<br><span style='font-size:12px; font-weight:normal;'>onSaveClicked</span>"]:::level1
    Undo["Undo<br><span style='font-size:12px; font-weight:normal;'>onUndoClicked</span>"]:::level1
    FilterBtn["Filter<br><span style='font-size:12px; font-weight:normal;'>onApplyClicked</span>"]:::level1
    View["View<br><span style='font-size:12px; font-weight:normal;'>ImageView class</span>"]:::level1

    Main --> Open
    Main --> Save
    Main --> Undo
    Main --> FilterBtn
    Main --> View

    subgraph L2 ["Level 2: filters called by onApplyClicked"]
        direction TB
        
        F1["F1 Grayscale<br><span style='font-size:12px; font-weight:normal;'>Grayscaleflow</span>"]:::adam
        F2["F2 Black and white<br><span style='font-size:12px; font-weight:normal;'>BlackandWhiteflow</span>"]:::youssef
        F3["F3 Invert<br><span style='font-size:12px; font-weight:normal;'>invertColorsflow</span>"]:::ibrahim
        F4["F4 Frame<br><span style='font-size:12px; font-weight:normal;'>addFrame</span>"]:::fares
        
        F5["F5 Flip<br><span style='font-size:12px; font-weight:normal;'>flipImage</span>"]:::adam
        F6["F6 Rotate<br><span style='font-size:12px; font-weight:normal;'>rotateImage</span>"]:::youssef
        F7["F7 Darken / lighten<br><span style='font-size:12px; font-weight:normal;'>adjustBrightness</span>"]:::ibrahim
        F8["F8 Resize<br><span style='font-size:12px; font-weight:normal;'>resizeImage</span>"]:::fares

        F9["F9 Merge<br><span style='font-size:12px; font-weight:normal;'>mergeImages, uses F8</span>"]:::adam
        F10["F10 Edge detection<br><span style='font-size:12px; font-weight:normal;'>detectEdges</span>"]:::youssef
        F11["F11 Crop<br><span style='font-size:12px; font-weight:normal;'>cropImage</span>"]:::ibrahim
        F12["F12 Blur<br><span style='font-size:12px; font-weight:normal;'>blurWithLevel</span>"]:::fares

        F13["F13 Wano sunlight<br><span style='font-size:12px; font-weight:normal;'>wanoSunlightFlow</span>"]:::adam
        F14["F14 Old TV<br><span style='font-size:12px; font-weight:normal;'>oldTV</span>"]:::youssef
        F15["F15 Wano night<br><span style='font-size:12px; font-weight:normal;'>purpleFilter</span>"]:::ibrahim
        F16["F16 Infrared<br><span style='font-size:12px; font-weight:normal;'>infrared</span>"]:::fares

        F17["F17 Skew<br><span style='font-size:12px; font-weight:normal;'>skewImage</span>"]:::team
        F18["F18 Oil painting<br><span style='font-size:12px; font-weight:normal;'>oilPaint</span>"]:::team
    end

    FilterBtn --> L2
