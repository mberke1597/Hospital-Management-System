#ifndef TRIAGEENTRY_H
#define TRIAGEENTRY_H
#include<iostream>

struct TriageEntry{
    int priorityLevel;
    std::string patientID;
    std::string patientName;
    std::string doctorID;
    std::string timeSlot;
    int order;
};

struct TriageComparator {
    bool operator()(const TriageEntry& a, const TriageEntry& b) const {
        if (a.priorityLevel == b.priorityLevel)
            return a.order > b.order; 
        return a.priorityLevel > b.priorityLevel;
    }
};

#endif

