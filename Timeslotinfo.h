#ifndef TIMESLOTINFO_H
#define TIMESLOTINFO_H
#include <vector>
#include <string>
#include <iostream>
#include<queue>
#include "Patient.h"

#include "Appointment.h"

class TimeSlotInfo{
    private:
        std::string timeSlot;
        int capacity;
        std::vector<Appointment> appointments;
        std::queue<Patient> waitingList;
    public:
        TimeSlotInfo(const std::string ts,int c) : timeSlot(ts),capacity(c){};
        bool isFullAppointments(){
            if(appointments.size() == capacity){
                return true;
            }else{
                return false;
            };
        };
        bool isFullWaitingList(){
            if(waitingList.size() >= capacity){
                return true;
            }else{
                return false;
            };
        };
        bool addAppointment(const Appointment& a){
            if(isFullAppointments()){
                // Guard: slot is full, caller should handle waiting list.
                return false;
            };
            appointments.push_back(a);
            return true;
        };
        bool removeAppointmentByPatientID(const std::string& patientID1){
            if(appointments.empty()){
                std::cout<<"There are no appointments in this slot to remove."<<std::endl;
                return false;
            };
            for(size_t i = 0 ; i < appointments.size() ; i++ ){
                if(appointments[i].getPatientID() == patientID1){
                    std::cout<<"Patient Appointment find."<<std::endl;
                    appointments.erase(appointments.begin() + i);
                    return true;
                };
            };
            std::cout<<"There is no appointment on this Patient ID."<<std::endl;
            return false;    
        };
        void addToWaitingList(const Patient& p){
            if(isFullWaitingList()){
                std::cout<<"Waiting List is Full .We can't add!"<<std::endl;
                return;
            };
            waitingList.push(p);    
        };
        Patient popFromWaitingList(){
            if(waitingList.empty()){
                // Return-by-exception is OK here; callers check emptiness before calling.
                throw std::runtime_error("Error: Waiting List is empty!");
            };
            Patient p = waitingList.front();
            waitingList.pop();
            return p;
        };
        int getCapacity() const { return capacity; }
        size_t getAppointmentCount() const { return appointments.size(); }
        const std::vector<Appointment>& getAppointments() const { return appointments; };
        const std::queue<Patient>& getWaitingList() const { return waitingList; };
};

#endif