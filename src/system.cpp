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
    //std::cout << "SYSTEM: Tamaño processes_: " << processes_.size() << "\n";
}

System::System(std::vector<Process> &processes) : processes_(processes) {
    // Characteristic that doesn't change during program execution. 
    kernel_ = LinuxParser::Kernel();
    operating_system_ = LinuxParser::OperatingSystem();
    //std::cout << "SYSTEM: Tamaño processes_: " << processes_.size() << "\n";
}


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::SortProcesses() {
    std::sort(processes_.begin(), processes_.end());
}


vector<Process> &System::Processes() { 
    
    vector<int> pids{};
    vector<Process> processes_temp{};
    // Nuevos procesos o actualizo los que existen
    for(int pid : LinuxParser::Pids()){
        pids.emplace_back(pid);
        const Process &myprocess{pid};
       // std::cout << "(" << pid << ") ";
        
        auto it = std::find(processes_.begin(), processes_.end(), myprocess);
        if (it != processes_.end()) {
        //Proceso ya existe

        // if (std::find(processes_.begin(), processes_.end(), myprocess) != processes_.end() ){
            // 
            // int index = it - processes_.begin();
            // //Process myprocess = processes_[index];
            // // if(processes_[index].Pid() == 2531){
            // //     std::cout << "activeJiffiesPrev: " << processes_[index].getActiveJiffiesPrev() << " ";
            // // }
            
            // Process *myprocess1 = processes_.data();
            // myprocess1[index].CpuUtilizationCalc();
            
            // // std::cout << "CPU: " << processes_[index].CpuUtilization() << "[]";

            // //vector<Process> test = {myprocess};
            // //processes_.assign (it,processes_.begin()+index);
            // //processes_.assign (it,myprocess);
            // //processes_.assign(1,myprocess);
            
            // //myprocess.CpuUtilizationCalc();
            // processes_.emplace_back(myprocess1[index]);
            // processes_.erase(processes_.begin()+index);
            
        } else {
            //Nuevo proceso
            Process new_process{pid};
            new_process.CpuUtilizationCalc();
            processes_.emplace_back(new_process);
            //std::cout << "Meto proceso" << "(" << ") ";
            //std::cout << "Cpu Utilization: " << processes_.back().CpuUtilization() << " ";
            
        }
    }

    //std::cout << "\n\n\n---------------------INTRO--------------- " << "  \n\n\n";
    //Elimino procesos obsoletos
    for (long unsigned int i = 0; i < processes_.size(); i++){
        //std::cout << "Cpu Utilization: " << processes_[i].CpuUtilization() << "\n";
        
        if (std::find(pids.begin(), pids.end(), processes_[i].Pid()) != pids.end()) {
        //Si el proceso se corresponde con uno de los PIDS, es que existe. 
     
        } else {
            //No hago nada, ya tendré mi vector temporal.
            //std::cout << "Elimino proceso: " << processes_.size() << "\n";
            processes_.erase(processes_.begin()+i);
        }    
    }

 

    //processes_ = processes_temp;

    // order process by CPU utilization.
    std::sort(processes_.begin(), processes_.end()); // This use the < operator implemented un process.cpp
    // for (Process process : processes_) {
    //     if (process.CpuUtilization()*100 != 0){
    //         std::cout << "CPU: " << process.CpuUtilization()*100 << " ";
    //     }
    // }
    
    
    //std::this_thread::sleep_for(std::chrono::seconds(20));
    return processes_; 
}

std::string System::Kernel() { return kernel_; }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() { return operating_system_; }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses();}


long int System::UpTime() { return LinuxParser::UpTime(); }