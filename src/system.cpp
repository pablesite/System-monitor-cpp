#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h" //Included by me
#include <iostream>
//TEST
#include <curses.h>
#include <chrono>
#include <string>
#include <thread>

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    // Characteristic that doesn't change during program execution. 
    kernel_ = LinuxParser::Kernel();
    operating_system_ = LinuxParser::OperatingSystem();
}


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::SortProcesses() {
    std::sort(processes_.begin(), processes_.end());
}


vector<Process> &System::Processes() { 
    
    vector<int> pids{};
    // Nuevos procesos o actualizo los que existen
    for(int pid : LinuxParser::Pids()){
        pids.emplace_back(pid);

        const Process &search_process{pid};
        auto it = std::find(processes_.begin(), processes_.end(), search_process);
        if (it != processes_.end()) {
            //Proceso ya existe
            Process *pointer_process = processes_.data();
            pointer_process[it - processes_.begin()].CpuUtilizationCalc();
            
        } else {
            //Nuevo proceso
            Process new_process{pid};
            new_process.CpuUtilizationCalc();
            processes_.emplace_back(new_process);
        }
    }

    //Elimino procesos obsoletos
    for (long unsigned int i = 0; i < processes_.size(); i++){       
        if (std::find(pids.begin(), pids.end(), processes_[i].Pid()) == pids.end()) {
        //Si el proceso no se corresponde con ningún pid lo elimino. 
            processes_.erase(processes_.begin()+i);
        }  
    }

    // order process by CPU utilization.
    std::sort(processes_.begin(), processes_.end()); // This use the < operator implemented un process.cpp
    
    return processes_; 
}

std::string System::Kernel() { return kernel_; }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() { return operating_system_; }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses();}


long int System::UpTime() { return LinuxParser::UpTime(); }