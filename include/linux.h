#ifndef LINUX_H
#define LINUX_H

#include <string>
#include <vector>

#include "linux_process.h"
#include "linux_processor.h"
#include "system.h"

class Linux : public System {
public:
  Linux();

  /*****Return private members*****/
  std::string Kernel() const override;
  std::string OperatingSystem() const override;

  /*****Return dinamic data from linux parser*****/
  float MemoryUtilization() const override;
  long UpTime() const override;
  int TotalProcesses() const override;
  int RunningProcesses() const override;

  /*****Components of a System: Processor and a container of Processes *****/
  LinuxProcessor &Cpu() override;
  std::vector<LinuxProcess> &Processes();

private:
  /*****To be returned through member functions*****/
  std::string kernel_;
  std::string operating_system_;
  LinuxProcessor cpu_{};
  std::vector<LinuxProcess> processes_{};
};

#endif