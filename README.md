# 🏥 Hospital Appointment and Triage System

**Course:** CMPE 2105 – Data Structures and Algorithms  
**Language:** C++  
**Repository:** https://github.com/mberke1597/DSAProject

---

## 📌 Project Overview

This project implements a **console-based Hospital Appointment and Triage Management System** using C++ and fundamental **data structures**.  
The main goal is to simulate a real-world hospital workflow while demonstrating correct and effective use of STL containers such as `map`, `vector`, `queue`, `priority_queue`, and `stack`.

The system allows hospital staff to:
- Manage doctors and their schedules
- Schedule and cancel patient appointments
- Maintain waiting lists for fully booked time slots
- Decide which patient should be called next using a triage priority system

All data is stored **in memory only** (no database or file I/O).

---

## ⚙️ Features

- Add and list doctors
- View a doctor’s schedule by time slot
- Schedule appointments with automatic time slot creation
- Waiting list management (FIFO)
- Cancel appointments with automatic promotion from waiting list
- Global triage system using priority queue
- Call next patient based on urgency
- (Optional) Undo last called patient using a stack
- Patient appointment lookup across all doctors

---

## 🧱 System Architecture & Class Design

### 1️⃣ Patient
Represents a patient in the hospital system.

**Attributes:**
- `patientID` (string)
- `name` (string)
- `priorityLevel` (int, 1 = highest urgency)

---

### 2️⃣ Appointment
Represents a confirmed appointment between a patient and a doctor.

**Attributes:**
- `doctorID`
- `patientID`
- `patientName`
- `timeSlot`
- `priorityLevel`

---

### 3️⃣ TimeSlotInfo
Stores all information related to a single doctor’s time slot.

**Attributes:**
- `timeSlot`
- `capacity`
- `appointments` → `vector<Appointment>`
- `waitingList` → `queue<Patient>`

**Responsibilities:**
- Check if slot is full
- Add/remove appointments
- Manage waiting list

---

### 4️⃣ Doctor
Represents a doctor and their schedule.

**Attributes:**
- `doctorID`
- `name`
- `specialty`
- `schedule` → `map<timeSlot, TimeSlotInfo>`

---

### 5️⃣ TriageEntry
Represents an entry in the global triage priority queue.

**Attributes:**
- `priorityLevel`
- `patientID`
- `patientName`
- `doctorID`
- `timeSlot`
- `order` (used to break ties)

Patients with **lower priorityLevel values are served first**.

---

### 6️⃣ HospitalSystem
Main manager class that controls the entire system.

**Responsibilities:**
- Doctor management
- Appointment scheduling & cancellation
- Waiting list handling
- Global triage queue management
- Menu-driven user interaction

---

## 🗂️ Data Structures Used (and Why)

| Data Structure | Where Used | Reason |
|---------------|----------|--------|
| `std::map` | Doctors, schedules | Fast lookup by ID or time slot |
| `std::vector` | Appointments | Dynamic storage, easy iteration |
| `std::queue` | Waiting lists | FIFO order for fairness |
| `std::priority_queue` | Global triage | Always access highest priority patient |
| `std::stack` (optional) | Undo call | LIFO undo mechanism |

---

## 👤 Author

**Muhammet Berke Ağaya**  
Computer Engineering Student  
Istanbul University

---

## 📂 Repository File Structure

The project is organized using separate header and source files as follows:

- `MBA_5001230021.cpp` – Main file containing `main()` and the menu-driven interface
- `HospitalSystem.h` – Declaration of the HospitalSystem class
- `HospitalSystem.cpp` – Implementation of the HospitalSystem logic
- `Doctor.h` – Doctor class definition
- `Patient.h` – Patient class definition
- `Appointment.h` – Appointment class definition
- `Timeslotinfo.h` – TimeSlotInfo class definition
- `TriageEntry.h` – TriageEntry struct and priority queue comparator

---
