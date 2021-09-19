#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();

  std::string Kernel() const;               
  std::string OperatingSystem() const; 

  float MemoryUtilization() const;          
  long UpTime() const;                      
  int TotalProcesses() const;               
  int RunningProcesses() const;   

  Processor& Cpu();                  
  std::vector<Process>& Processes();      


 private:
  //private members to be returned througt members function
  std::string kernel_;
  std::string operating_system_;
  Processor cpu_{};
  std::vector<Process> processes_{};
  
};

#endif