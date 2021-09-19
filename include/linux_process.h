#ifndef LINUX_PROCESS_H
#define LINUX_PROCESS_H

#include <string>
#include <vector>

#include "process.h"

class LinuxProcess : public Process {
public:
  LinuxProcess(int pid);
  int Pid() const override;
  std::string User() const override;
  std::string Command() const override;
  float CpuUtilization() const override;

  long int UpTime() const override;
  std::string Ram() const override;

  void CalcCpuUtilization(long unsigned int time_acc) override;

  bool operator<(LinuxProcess const &a) const;
  bool operator==(LinuxProcess const &a) const;

private:
  // private members to be returned througt members function
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_utilization_;

  // private members to be used internally to calculate cpu utilization.
  long active_jiffies_prev_{0};
  long seconds_prev_{0};
  std::vector<long> active_jiffies_acc_{10, 0};
  std::vector<long> seconds_acc_{10, 0};
};

#endif