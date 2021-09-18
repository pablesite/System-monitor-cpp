#include "processor.h"
#include "linux_parser.h" //Included by me
#include <iostream>

// CPU Utilization is the diferential of active jiffes divided by diferential of total jiffies.
float Processor::Utilization() { 
    long active_j = LinuxParser::ActiveJiffies();

    // Calculation of spreads.
    long active_d = active_j - active_j_prev_;
    long total_d = LinuxParser::Jiffies() - active_j_prev_ - idle_j_prev_;

    // Update previous data for next calculation.
    active_j_prev_ = active_j;
    idle_j_prev_ = LinuxParser::IdleJiffies();                                       
     
    return (float)active_d/(float)total_d; 
}

