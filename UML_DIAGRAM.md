# Hospital Management System - UML Class Diagram

## Mermaid UML Diagram

```mermaid
classDiagram
    class HospitalSystem {
        -map~string, Doctor~ doctors
        -priority_queue~TriageEntry~ triageQueue
        -int triageOrderCounter
        -loadDoctorsFromFile()
        -loadAppointmentsFromFile()
        -rewriteAppointmentsFile()
        -appendDoctorToFile(Doctor)
        +HospitalSystem()
        +isValidTime(string) bool
        +addDoctor(Doctor)
        +listDoctors()
        +viewDoctorSchedule(string)
        +scheduleAppointment(Patient, string, string)
        +cancelAppointment(string, string, string)
        +callNextPatient(string)
        +listAppointmentsFor(string, string)
        +runMenu()
    }

    class Doctor {
        -string doctorID
        -string name
        -string specialty
        -map~string, TimeSlotInfo~ schedule
        +Doctor(string, string, string)
        +addTimeSlot(string, int)
        +getTimeSlot(string) TimeSlotInfo*
        +printSchedule()
        +getdName() string
        +getdSpecialty() string
        +getdID() string
        +getSchedule() map~string, TimeSlotInfo~
    }

    class Patient {
        -string patientID
        -string namePAT
        -int priorityLevel
        +Patient(string, string, int)
        +getpID() string
        +getnamePat() string
        +getpriLevel() int
        +setpriLevel(int)
        +setnamePat(string)
        +setPID(string)
    }

    class Appointment {
        -string doctorID
        -string patientID
        -string patientName
        -string timeSlot
        -int priLevel
        +Appointment(string, string, string, string, int)
        +getPatientID() string
        +getPatientName() string
        +getPriorityLevel() int
    }

    class TimeSlotInfo {
        -string timeSlot
        -int capacity
        -vector~Appointment~ appointments
        -queue~Patient~ waitingList
        +TimeSlotInfo(string, int)
        +isFullAppointments() bool
        +isFullWaitingList() bool
        +addAppointment(Appointment) bool
        +removeAppointmentByPatientID(string) bool
        +addToWaitingList(Patient)
        +popFromWaitingList() Patient
        +getCapacity() int
        +getAppointmentCount() size_t
        +getAppointments() vector~Appointment~
        +getWaitingList() queue~Patient~
    }

    class TriageEntry {
        +int priorityLevel
        +string patientID
        +string patientName
        +string doctorID
        +string timeSlot
        +int order
    }

    class TriageComparator {
        +operator()(TriageEntry, TriageEntry) bool
    }

    %% Relationships
    HospitalSystem "1" *-- "0..*" Doctor : manages
    HospitalSystem "1" o-- "0..*" TriageEntry : uses
    HospitalSystem ..> TriageComparator : uses
    HospitalSystem ..> Patient : uses
    HospitalSystem ..> Appointment : uses
    
    Doctor "1" *-- "0..*" TimeSlotInfo : contains
    
    TimeSlotInfo "1" o-- "0..*" Appointment : stores
    TimeSlotInfo "1" o-- "0..*" Patient : queues
    
    Appointment ..> Patient : references
    
    TriageEntry ..> Patient : references
    TriageEntry ..> Doctor : references
    
    TriageComparator ..> TriageEntry : compares
```

## PlantUML Diagram (Alternative Format)

```plantuml
@startuml Hospital Management System

class HospitalSystem {
    - doctors : map<string, Doctor>
    - triageQueue : priority_queue<TriageEntry>
    - triageOrderCounter : int
    - loadDoctorsFromFile() : void
    - loadAppointmentsFromFile() : void
    - rewriteAppointmentsFile() : void
    - appendDoctorToFile(Doctor) : void
    + HospitalSystem()
    + isValidTime(string) : bool
    + addDoctor(Doctor) : void
    + listDoctors() : void
    + viewDoctorSchedule(string) : void
    + scheduleAppointment(Patient, string, string) : void
    + cancelAppointment(string, string, string) : void
    + callNextPatient(string) : void
    + listAppointmentsFor(string, string) : void
    + runMenu() : void
}

class Doctor {
    - doctorID : string
    - name : string
    - specialty : string
    - schedule : map<string, TimeSlotInfo>
    + Doctor(string, string, string)
    + addTimeSlot(string, int) : void
    + getTimeSlot(string) : TimeSlotInfo*
    + printSchedule() : void
    + getdName() : string
    + getdSpecialty() : string
    + getdID() : string
    + getSchedule() : map<string, TimeSlotInfo>
}

class Patient {
    - patientID : string
    - namePAT : string
    - priorityLevel : int
    + Patient(string, string, int)
    + getpID() : string
    + getnamePat() : string
    + getpriLevel() : int
    + setpriLevel(int) : void
    + setnamePat(string) : void
    + setPID(string) : void
}

class Appointment {
    - doctorID : string
    - patientID : string
    - patientName : string
    - timeSlot : string
    - priLevel : int
    + Appointment(string, string, string, string, int)
    + getPatientID() : string
    + getPatientName() : string
    + getPriorityLevel() : int
}

class TimeSlotInfo {
    - timeSlot : string
    - capacity : int
    - appointments : vector<Appointment>
    - waitingList : queue<Patient>
    + TimeSlotInfo(string, int)
    + isFullAppointments() : bool
    + isFullWaitingList() : bool
    + addAppointment(Appointment) : bool
    + removeAppointmentByPatientID(string) : bool
    + addToWaitingList(Patient) : void
    + popFromWaitingList() : Patient
    + getCapacity() : int
    + getAppointmentCount() : size_t
    + getAppointments() : vector<Appointment>
    + getWaitingList() : queue<Patient>
}

struct TriageEntry {
    + priorityLevel : int
    + patientID : string
    + patientName : string
    + doctorID : string
    + timeSlot : string
    + order : int
}

struct TriageComparator {
    + operator()(TriageEntry, TriageEntry) : bool
}

' Relationships
HospitalSystem "1" *-- "0..*" Doctor : manages
HospitalSystem "1" o-- "0..*" TriageEntry : uses priority queue
HospitalSystem ..> TriageComparator : uses for sorting
HospitalSystem ..> Patient : uses
HospitalSystem ..> Appointment : uses

Doctor "1" *-- "0..*" TimeSlotInfo : contains schedule

TimeSlotInfo "1" o-- "0..*" Appointment : stores
TimeSlotInfo "1" o-- "0..*" Patient : queues in waiting list

Appointment ..> Patient : references data
Appointment ..> Doctor : references

TriageEntry ..> Patient : references
TriageEntry ..> Doctor : references

TriageComparator ..> TriageEntry : compares

@enduml
```

## Relationship Descriptions

### Composition (Filled Diamond - Strong Ownership)
- **HospitalSystem *-- Doctor**: HospitalSystem owns and manages Doctor objects. When HospitalSystem is destroyed, all Doctors are destroyed.
- **Doctor *-- TimeSlotInfo**: Doctor owns its TimeSlotInfo objects in the schedule map. When Doctor is destroyed, all TimeSlots are destroyed.

### Aggregation (Empty Diamond - Weak Ownership)
- **HospitalSystem o-- TriageEntry**: HospitalSystem contains TriageEntry objects in priority queue, but they can exist independently.
- **TimeSlotInfo o-- Appointment**: TimeSlotInfo contains Appointment objects, but appointments reference external patient data.
- **TimeSlotInfo o-- Patient**: TimeSlotInfo contains Patient objects in waiting list, but patients exist independently.

### Dependency (Dotted Arrow - Uses)
- **HospitalSystem ..> Patient**: HospitalSystem uses Patient objects in operations.
- **HospitalSystem ..> Appointment**: HospitalSystem uses Appointment objects.
- **HospitalSystem ..> TriageComparator**: HospitalSystem uses comparator for priority queue.
- **Appointment ..> Patient**: Appointment references patient information.
- **Appointment ..> Doctor**: Appointment references doctor information.
- **TriageEntry ..> Patient**: TriageEntry references patient data.
- **TriageEntry ..> Doctor**: TriageEntry references doctor ID.
- **TriageComparator ..> TriageEntry**: Comparator operates on TriageEntry objects.

## Key Design Patterns Visible in UML

### 1. Facade Pattern
**HospitalSystem** acts as a facade, providing simplified interface to complex subsystem of Doctor, Patient, Appointment, and TimeSlotInfo interactions.

### 2. Container Pattern
- **Doctor** contains multiple **TimeSlotInfo** objects (map container)
- **TimeSlotInfo** contains multiple **Appointment** objects (vector container)
- **TimeSlotInfo** contains multiple **Patient** objects in waiting list (queue container)
- **HospitalSystem** contains multiple **Doctor** objects (map container)

### 3. Strategy Pattern
**TriageComparator** defines comparison strategy for **TriageEntry** objects in priority queue.

### 4. Data Transfer Object (DTO)
**TriageEntry** and **Appointment** serve as DTOs carrying data between system components.

## Class Responsibilities

| Class | Primary Responsibility | Key Characteristic |
|:------|:----------------------|:-------------------|
| **HospitalSystem** | System Controller | Orchestrates all operations, manages doctors, handles triage |
| **Doctor** | Entity | Represents doctor with schedule management |
| **Patient** | Entity | Represents patient data |
| **Appointment** | Value Object | Immutable appointment record linking patient, doctor, time |
| **TimeSlotInfo** | Resource Manager | Manages capacity, appointments, and waiting list for time slot |
| **TriageEntry** | Data Structure | Priority queue element for patient calling |
| **TriageComparator** | Comparator | Defines priority ordering logic |

## Multiplicity Details

- **HospitalSystem → Doctor**: 1 to many (0..*)
  - One system manages multiple doctors
  
- **Doctor → TimeSlotInfo**: 1 to many (0..*)
  - One doctor has multiple time slots
  
- **TimeSlotInfo → Appointment**: 1 to many (0..*)
  - One time slot can have multiple appointments (up to capacity)
  
- **TimeSlotInfo → Patient**: 1 to many (0..*)
  - One time slot can have multiple patients in waiting list
  
- **HospitalSystem → TriageEntry**: 1 to many (0..*)
  - One system manages priority queue of triage entries

## Data Flow Diagram

```
User Input
    ↓
┌─────────────────────┐
│  HospitalSystem     │
│  (Facade/Controller)│
└──────────┬──────────┘
           │
           ├──→ Doctor ──→ TimeSlotInfo ──→ Appointment
           │                    │
           │                    └──→ Patient (waiting list)
           │
           └──→ TriageQueue (TriageEntry + TriageComparator)
                    ↓
              Patient Calling Order
```

## File Dependencies

```
MBA_5001230021.cpp (main)
    └── includes HospitalSystem.h
            ├── includes Patient.h
            ├── includes Doctor.h
            │       └── includes Timeslotinfo.h
            │               ├── includes Patient.h
            │               └── includes Appointment.h
            ├── includes Timeslotinfo.h (already included by Doctor.h)
            ├── includes Appointment.h (already included by Timeslotinfo.h)
            └── includes TriageEntry.h
```

---

*Generated UML Diagram for Hospital Management System*
*Date: December 29, 2025*
