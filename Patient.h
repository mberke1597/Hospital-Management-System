#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>

class Patient{
    private:
        std::string patientID;
        std::string namePAT;
        int priorityLevel;
    public:
        Patient(std::string pID,std::string namePatient,int PriLevel){
            patientID = pID;
            namePAT = namePatient;
            priorityLevel = PriLevel;
        };
        const std::string getpID() const{
            return patientID;
        };
        
        const std::string getnamePat() const{
            return namePAT;
        };
        const int getpriLevel() const{
            return priorityLevel;
        };
        void setpriLevel(int inputpriLevel){
            priorityLevel = inputpriLevel;
        };
        void setnamePat(std::string nameInput){
            namePAT = nameInput;
        };
        void setPID(std::string PIDInput){
            patientID = PIDInput;
        };
        friend std::ostream& operator<<(std::ostream& os, const Patient& p);
};

inline std::ostream& operator<<(std::ostream& os, const Patient& p) {
    os << "Name: " << p.namePAT << ", Patient ID : " << p.patientID << "Priority Level: "<< p.priorityLevel<<std::endl;
    return os;
}

#endif