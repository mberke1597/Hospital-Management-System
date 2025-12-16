#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include<iostream>
#include<string>

class Appointment{
    private:
        std::string doctorID;
        std::string patientID;
        std::string patientName; // may be duplicated for convenience
        std::string timeSlot; // e.g., "09:00"
        int priLevel;
    public:
        Appointment(const std::string& doctorID,
                const std::string& patientID,
                const std::string& patientName,
                const std::string& timeSlot,
                int priorityLevel)
        : doctorID(doctorID),
          patientID(patientID),
          patientName(patientName),
          timeSlot(timeSlot),
          priLevel(priorityLevel) {};

        const std::string& getPatientID() const { return patientID; }
        const std::string& getPatientName() const { return patientName; }
        int getPriorityLevel() const { return priLevel; }
};

#endif
