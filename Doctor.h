#ifndef DOCTOR_H
#define DOCTOR_H
#include <vector>
#include <string>
#include <iostream>
#include "Timeslotinfo.h"
#include <map>

class Doctor{
    private:
        std::string doctorID;
        std::string name;
        std::string specialty;  
        std::map<std::string, TimeSlotInfo> schedule;
        
    public:
        
        Doctor(std::string dID, std::string nameDoctor, std::string specialtyDoctor)
        : doctorID(dID), name(nameDoctor), specialty(specialtyDoctor) {
            // Note: uniqueness check (doctorID) is handled in HospitalSystem.
        };
        void addTimeSlot(const std::string& timeSlot, int capacity){
            schedule.emplace(timeSlot, TimeSlotInfo(timeSlot, capacity));
        };
        TimeSlotInfo* getTimeSlot(const std::string& timeSlot){
            auto it = schedule.find(timeSlot);
            if(it == schedule.end()){
                return nullptr;
            };
            return &it->second;
        };
        void printSchedule() const{
            for(const auto& p : schedule){
                const TimeSlotInfo& ts = p.second;
                // Show capacity and current usage for each timeslot
                std::cout<<"Time Slot Capacity: " << ts.getCapacity() << "\n";
                std::cout<<"Appointments ( "<<ts.getAppointmentCount()<<" ) at "<<p.first<<"\n";
                for (const auto& a : ts.getAppointments()) {
                std::cout << "  " << a.getPatientID()
                          << " - " << a.getPatientName()
                          << " (priority " << a.getPriorityLevel() << ")\n";
                }
                std::cout << "Waiting List:\n";
                std::queue<Patient> temp = ts.getWaitingList();
                while (!temp.empty()) {
                    Patient p = temp.front();
                    temp.pop();
                    std::cout << "  " << p.getpID()
                              << " - " << p.getnamePat()
                              << " (priority " << p.getpriLevel() << ")\n";
                }
            };
            
        };
        const std::string getdName() const{
            return name;
        };
        const std::string getdSpecialty() const{
            return specialty;
        };
        const std::string getdID() const{
            return doctorID;
        };
         const std::map<std::string, TimeSlotInfo>& getSchedule() const { return schedule; };
};

#endif

