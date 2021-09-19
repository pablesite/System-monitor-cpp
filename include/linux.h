#ifndef LINUX_H
#define LINUX_H

#include <string>
#include <vector>

#include "system.h"
#include "process.h"
#include "linux_process.h"
#include "linux_processor.h"

class Linux : public System {
public:
  Linux();

  std::string Kernel() const override;
  std::string OperatingSystem() const override;

  float MemoryUtilization() const override;
  long UpTime() const override;
  int TotalProcesses() const override;
  int RunningProcesses() const override;

  LinuxProcessor &Cpu() override;
  std::vector<LinuxProcess> &Processes();

  // LinuxProcess &Test();

private:
  // private members to be returned througt members function
  std::string kernel_;
  std::string operating_system_;
  LinuxProcessor cpu_{};
  std::vector<LinuxProcess> processes_{};
};

#endif