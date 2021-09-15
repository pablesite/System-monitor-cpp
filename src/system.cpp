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

vector<Process>& System::Processes() { 
    // chequeo si ya existe en la lista de procesos
    for(int pid : LinuxParser::Pids()){
        bool exist = false;
        for (auto process : processes_){
            if(process.Pid() == pid){
                exist = true;
            }
        }
        
        if(!exist){
            //std::cout << "Meto el proceso: " << pid << "\n";
            Process myprocess{pid};
            processes_.emplace_back(myprocess);
        }
                     
    }
    // Comprobar que no hay PIDs en la lista que ya hayan desaparecido.
    // probablemente haciendo el for al contrario:
    //for processes_ --> for LinuxParser::Pids --> if not exist --> erase process
    //for (auto process : processes_){
        //process.CpuUtilization();
        // if (process.Pid() == 1788){
        //     std::cout << "ID: " << process.Pid() << "(" << process.getActiveJiffiesPrev() << ")" ;
        // }
        // bool process_exist = false;
        // for (auto pid : LinuxParser::Pids()){
        //     if(process.Pid() == pid) {
        //         process_exist = true;
        //     }
        // }
        // if (!process_exist) {
        //     Process* it = std::find(processes_.begin(), processes_.end(), process.Pid());
        //     //int index = std::distance(processes_.begin(), it);
        //     //processes_.erase(processes_.begin() + index);
        // }
        
   // }
    

    // order process by CPU utilization.
    std::sort(processes_.begin(), processes_.end()); // This use the < operator implemented un process.cpp

    // std::cout << "\n" << " INICIO  ";
    // for (auto process : processes_){
    //     std::cout << process.CpuUtilization()*100 << " ";
    // }
    // std::cout << " FIN  \n";

    return processes_; 
}

std::string System::Kernel() { return kernel_; }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() { return operating_system_; }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses();}


long int System::UpTime() { return LinuxParser::UpTime(); }