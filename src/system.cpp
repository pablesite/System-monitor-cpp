#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h" 

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    // Characteristic that doesn't change during program execution. 
    kernel_ = LinuxParser::Kernel();
    operating_system_ = LinuxParser::OperatingSystem();
}

// Const functions.
std::string System::Kernel() const { return kernel_; }

std::string System::OperatingSystem() const { return operating_system_; }

// Parser functions.
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses();}

long int System::UpTime() const { return LinuxParser::UpTime(); }

// Componentes. (otras clases)
// TODO: Return the system's CPU
Processor& System::Cpu()  { return cpu_; }

vector<Process>& System::Processes() { 
    
    vector<int> pids{};
    // Nuevos procesos o actualizo los que existen
    for(int pid : LinuxParser::Pids()){
        pids.emplace_back(pid);

        const Process &search_process{pid};
        auto it = std::find(processes_.begin(), processes_.end(), search_process);
        if (it != processes_.end()) {
            //Proceso ya existe
            Process *pointer_process = processes_.data();
            pointer_process[it - processes_.begin()].CalcCpuUtilization(10);
            
        } else {
            //Nuevo proceso
            Process new_process{pid};
            new_process.CalcCpuUtilization(1); //time_acc debería ser para toda la clase...?
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