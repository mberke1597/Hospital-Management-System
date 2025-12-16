#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H

#include<iostream>
#include<string>
#include<queue>
#include<map>
#include<fstream>
#include "Patient.h"
#include "Doctor.h"
#include "Timeslotinfo.h"
#include "Appointment.h"
#include "TriageEntry.h"
#include <limits>
#include <vector>


class HospitalSystem{
    private:
        std::map<std::string, Doctor> doctors;
        std::priority_queue<TriageEntry, std::vector<TriageEntry>, TriageComparator> triageQueue;
        int triageOrderCounter;

        void loadDoctorsFromFile();
        void loadAppointmentsFromFile();
        void rewriteAppointmentsFile() const;
        void appendDoctorToFile(const Doctor& d) const;

    public:
        HospitalSystem();
        
        bool isValidTime(const std::string& time) const;
        void addDoctor(const Doctor& d);
        void listDoctors() const;
        void viewDoctorSchedule(const std::string& doctorID) const;
        void scheduleAppointment(const Patient& p, const std::string& doctorID, const std::string& timeSlot);
        void cancelAppointment(const std::string& doctorID, const std::string& timeSlot, const std::string& patientID);
        void callNextPatient(const std::string& doctorID);
        void listAppointmentsFor(const std::string& doctorID, const std::string& timeSlot) const;
        void runMenu();
};
#endif