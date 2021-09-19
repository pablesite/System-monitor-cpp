#ifndef LINUX_PROCESS_H
#define LINUX_PROCESS_H

#include <string>
#include <vector>

#include "process.h"

class LinuxProcess : public Process {
public:
  LinuxProcess(int pid);

  /*****Return private members*****/
  int Pid() const override;
  std::string User() const override;
  std::string Command() const override;
  float CpuUtilization() const override;

  /*****Return dinamic data from linux parser*****/
  long int UpTime() const override;
  std::string Ram() const override;

  /*****Calculation of Cpu Utilization used in Linux::Processes()*****/
  void CalcCpuUtilization(long unsigned int time_acc) override;

  /*****Operators overloading*****/
  bool operator<(LinuxProcess const &a) const;
  bool operator==(LinuxProcess const &a) const;

private:
  /*****To be returned through member functions*****/
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_utilization_;

  /*****To be used to calculate cpu utilization of each process*****/
  long active_jiffies_prev_{0};
  long seconds_prev_{0};
  std::vector<long> active_jiffies_acc_{10, 0};
  std::vector<long> seconds_acc_{10, 0};
};

#endif