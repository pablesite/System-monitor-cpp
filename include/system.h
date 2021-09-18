#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  System(std::vector<Process > &processes);
  Processor& Cpu();                   // TODO: See src/system.cpp
  void SortProcesses();
  std::vector<Process> &Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process > processes_{};
  std::string kernel_;
  std::string operating_system_;
};

#endif