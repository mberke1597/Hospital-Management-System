#include "HospitalSystem.h"

HospitalSystem::HospitalSystem() : triageOrderCounter(0) {
    loadDoctorsFromFile();
    loadAppointmentsFromFile();
}

bool HospitalSystem::isValidTime(const std::string& time) const {
    if (time.length() != 5 || time[2] != ':') return false;
    if (!isdigit(time[0]) || !isdigit(time[1]) ||
        !isdigit(time[3]) || !isdigit(time[4])) return false;

    int hours = (time[0]-'0')*10 + (time[1]-'0');
    int minutes = (time[3]-'0')*10 + (time[4]-'0');

    return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}

void HospitalSystem::addDoctor(const Doctor& d) {
    if(doctors.count(d.getdID())){
        std::cout<<"Doctor ID is exists"<<std::endl;
        return;
    }
    doctors.emplace(d.getdID(), d);
    appendDoctorToFile(d);
    std::cout<<"Doctor added successfully."<<std::endl;
}

void HospitalSystem::appendDoctorToFile(const Doctor& d) const {
    std::ofstream out("doctors.txt", std::ios::app);
    out << d.getdID() << " "
        << d.getdName() << " "
        << d.getdSpecialty() << "\n";
}

void HospitalSystem::listDoctors() const {
    if (doctors.empty()) {
        std::cout << "No doctors in the system.\n";
        return;
    }
    for (const auto& p : doctors) {
        std::cout << p.second.getdID() << " | "
                  << p.second.getdName() << " | "
                  << p.second.getdSpecialty()
                  << " | Appointments: ";

        int total = 0;
        for (const auto& s : p.second.getSchedule())
            total += s.second.getAppointments().size();

        std::cout << total << "\n";
    }
}

void HospitalSystem::viewDoctorSchedule(const std::string& doctorID) const {
    auto it = doctors.find(doctorID);
    if (it == doctors.end()) {
        std::cout << "Doctor not found.\n";
        return;
    }
    if (it->second.getSchedule().empty()) {
        std::cout << "No schedule available for this doctor.\n";
        return;
    }

    it->second.printSchedule();
}

void HospitalSystem::scheduleAppointment(const Patient& p,
                                        const std::string& doctorID,
                                        const std::string& timeSlot) {

    auto it = doctors.find(doctorID);
    if (it == doctors.end()) {
        std::cout << "Doctor not found.\n";
        return;
    }

    TimeSlotInfo* slot = it->second.getTimeSlot(timeSlot);
    if (!slot) {
        // Auto-create a new timeslot with default capacity 2 when missing
        it->second.addTimeSlot(timeSlot, 2);
        slot = it->second.getTimeSlot(timeSlot);
    }

    Appointment a(doctorID, p.getpID(), p.getnamePat(), timeSlot, p.getpriLevel());

    if (slot->addAppointment(a)) {
        triageQueue.push({p.getpriLevel(), p.getpID(), p.getnamePat(),
                          doctorID, timeSlot, triageOrderCounter++});

        std::cout << "Appointment scheduled.\n";
    } else {
        slot->addToWaitingList(p);
        std::cout << "Slot full. Added to waiting list.\n";
    }
    rewriteAppointmentsFile();
}

void HospitalSystem::rewriteAppointmentsFile() const {
    std::ofstream out("appointments.txt");

    for (const auto& dPair : doctors) {
        const Doctor& d = dPair.second;

        for (const auto& sPair : d.getSchedule()) {
            const std::string& ts = sPair.first;
            const TimeSlotInfo& slot = sPair.second;

            for (const auto& a : slot.getAppointments()) {
                out << d.getdID() << " "
                    << ts << " "
                    << a.getPatientID() << " "
                    << a.getPatientName() << " "
                    << a.getPriorityLevel() << "\n";
            }
        }
    }
}

void HospitalSystem::cancelAppointment(const std::string& doctorID,
                                      const std::string& timeSlot,
                                      const std::string& patientID) {

    auto it = doctors.find(doctorID);
    if (it == doctors.end()) {
        std::cout << "Doctor not found.\n";
        return;
    }

    TimeSlotInfo* slot = it->second.getTimeSlot(timeSlot);
    if (!slot){
        std::cout << "Time slot not found.\n";
        return;
    }

    if (!slot->removeAppointmentByPatientID(patientID)) {
        std::cout << "Appointment not found.\n";
        return;
    }

    std::cout << "Appointment cancelled.\n";

    if (!slot->getWaitingList().empty()) {
        Patient p = slot->popFromWaitingList();
        Appointment a(doctorID, p.getpID(), p.getnamePat(), timeSlot, p.getpriLevel());
        slot->addAppointment(a);

        triageQueue.push({p.getpriLevel(), p.getpID(), p.getnamePat(),
                          doctorID, timeSlot, triageOrderCounter++});

        std::cout << p.getnamePat() << " moved from waiting list.\n";
    }
    rewriteAppointmentsFile();
}

void HospitalSystem::loadDoctorsFromFile() {
    std::ifstream in("doctors.txt");
    if (!in) return;

    std::string id, name, spec;
    while (in >> id >> name >> spec) {
        doctors.emplace(id, Doctor(id, name, spec));
    }
}

void HospitalSystem::loadAppointmentsFromFile() {
    std::ifstream in("appointments.txt");
    if (!in) return;

    std::string doctorID, timeSlot, patientID, patientName;
    int priorityLevel;
    while (in >> doctorID >> timeSlot >> patientID >> patientName >> priorityLevel) {
        auto it = doctors.find(doctorID);
        if (it == doctors.end()) continue;

        TimeSlotInfo* slot = it->second.getTimeSlot(timeSlot);
        if (!slot) {
            it->second.addTimeSlot(timeSlot, 2);
            slot = it->second.getTimeSlot(timeSlot);
        }

        Appointment a(doctorID, patientID, patientName, timeSlot, priorityLevel);
        slot->addAppointment(a);
        
        // KRITIK FIX: Triage queue'yu doldur
        triageQueue.push({
            priorityLevel,
            patientID,
            patientName,
            doctorID,
            timeSlot,
            triageOrderCounter++
        });
    }
}

void HospitalSystem::callNextPatient(const std::string& doctorID) {

    auto docIt = doctors.find(doctorID);
    if (docIt == doctors.end()) {
        std::cout << "Doctor not found.\n";
        return;
    }

    if (triageQueue.empty()) {
        std::cout << "No patients waiting.\n";
        return;
    }

    std::priority_queue<TriageEntry,
        std::vector<TriageEntry>,
        TriageComparator> temp;

    while (!triageQueue.empty()) {
        TriageEntry current = triageQueue.top();
        triageQueue.pop();

        // Başka doktor → sakla
        if (current.doctorID != doctorID) {
            temp.push(current);
            continue;
        }

        // Timeslot var mı?
        auto slotIt = docIt->second.getSchedule().find(current.timeSlot);
        if (slotIt == docIt->second.getSchedule().end())
            continue;

        // Hasta hala randevulu mu?
        bool patientFound = false;
        for (const auto& appt : slotIt->second.getAppointments()) {
            if (appt.getPatientID() == current.patientID) {
                patientFound = true;
                break;
            }
        }

        if (!patientFound)
            continue;

        // ✅ DOĞRU HASTA
        std::cout << "\n=== NEXT PATIENT ===\n";
        std::cout << "Patient ID : " << current.patientID << "\n";
        std::cout << "Name       : " << current.patientName << "\n";
        std::cout << "Priority   : " << current.priorityLevel << "\n";
        std::cout << "Doctor     : " << current.doctorID << "\n";
        std::cout << "Time Slot  : " << current.timeSlot << "\n";

        docIt->second
            .getTimeSlot(current.timeSlot)
            ->removeAppointmentByPatientID(current.patientID);

        std::cout << "\nPatient has been called.\n";

        rewriteAppointmentsFile();

        // Geri kalanları geri koy
        while (!triageQueue.empty()) {
            temp.push(triageQueue.top());
            triageQueue.pop();
        }
        triageQueue = temp;

        return;
    }

    // Uygun hasta bulunamadıysa
    triageQueue = temp;
    std::cout << "No valid patients for this doctor.\n";
}


void HospitalSystem::listAppointmentsFor(const std::string& doctorID,
                                        const std::string& timeSlot) const {

    auto docIt = doctors.find(doctorID);
    if (docIt == doctors.end()) {
        std::cout << "Doctor not found.\n";
        return;
    }

    auto slotIt = docIt->second.getSchedule().find(timeSlot);
    if (slotIt == docIt->second.getSchedule().end()) {
        std::cout << "Time slot not found.\n";
        return;
    }

    const TimeSlotInfo& slot = slotIt->second;

    std::cout << "\n=== APPOINTMENTS ===\n";
    std::cout << "Doctor   : " << doctorID << "\n";
    std::cout << "TimeSlot : " << timeSlot << "\n";
    std::cout << "Capacity : " << slot.getCapacity() << "\n";
    std::cout << "Booked   : " << slot.getAppointmentCount() << "\n\n";

    if (slot.getAppointments().empty()) {
        std::cout << "No appointments.\n";
    } else {
        for (const auto& a : slot.getAppointments()) {
            std::cout << "- " << a.getPatientID()
                      << " | " << a.getPatientName()
                      << " | priority " << a.getPriorityLevel()
                      << "\n";
        }
    }

    std::cout << "\n--- Waiting List ---\n";
    if (slot.getWaitingList().empty()) {
        std::cout << "No patients waiting.\n";
    } else {
        std::queue<Patient> temp = slot.getWaitingList();
        while (!temp.empty()) {
            Patient p = temp.front();
            temp.pop();
            std::cout << "- " << p.getpID()
                      << " | " << p.getnamePat()
                      << " | priority " << p.getpriLevel()
                      << "\n";
        }
    }
}

void HospitalSystem::runMenu() {
    int choice;

    while (true) {
        std::cout << "\n===== Hospital System =====\n";
        std::cout << "1. Add Doctor\n";
        std::cout << "2. List Doctors\n";
        std::cout << "3. View Doctor Schedule\n";
        std::cout << "4. Schedule Appointment\n";
        std::cout << "5. Cancel Appointment\n";
        std::cout << "6. Call Next Patient\n";
        std::cout << "7. List Appointments for Doctor & TimeSlot\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting system...\n";
            break;
        }

        std::string id, name, spec, time;
        int priority;

        switch (choice) {

        case 1: {
            std::cout << "DoctorID Name Specialty: ";

            if (!(std::cin >> id >> name >> spec)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter: DoctorID Name Specialty\n";
                break;
            }

            addDoctor(Doctor(id, name, spec));
            break; 
        }

        case 2:
            listDoctors();
            break;

        case 3:
            std::cout << "DoctorID: ";
            std::cin >> id;
            viewDoctorSchedule(id);
            break;

        case 4: {
            std::cout << "PatientID Name Priority DoctorID TimeSlot: ";
            if (!(std::cin >> id >> name >> priority >> spec >> time)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input.\n";
                break;
            }

            if (priority < 0) {
                std::cout << "Priority must be >= 0.\n";
                break;
            }

            if (!isValidTime(time)) {
                std::cout << "Time format must be HH:MM\n";
                break;
            }

            scheduleAppointment(Patient(id, name, priority), spec, time);
            break;
        }

        case 5: {
            std::cout << "DoctorID TimeSlot PatientID: ";
            std::cin >> spec >> time >> id;
            cancelAppointment(spec, time, id);
            break;
        }

        case 6:
            std::cout << "DoctorID: ";
            std::cin >> id;
            callNextPatient(id);
            break;
        case 7: {
            std::cout << "DoctorID TimeSlot: ";
            std::cin >> id >> time;
            listAppointmentsFor(id, time);
            break;
        }
        default:
            std::cout << "Invalid choice.\n";
        }
    }
}
