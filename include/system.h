#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
  virtual std::string Kernel() const = 0;
  virtual std::string OperatingSystem() const = 0;

  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;

  virtual Processor &Cpu() = 0;
  /* I don't know how to make this function virtual functions.
     It use a Process as input, and it would be necesseray pass a LinuxProcess
     when it is overriden. This produce an error despite LinuxProcess derive
     from Process */
  // virtual std::vector<Process> &Processes() = 0;
};

#endif