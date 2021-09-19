#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
  virtual std::string Kernel() const = 0;
  virtual std::string OperatingSystem() const= 0;

  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;

  virtual Processor &Cpu() = 0;
  //virtual std::vector<Process> &Processes() = 0;
  //virtual Process &Test() = 0;

};

#endif