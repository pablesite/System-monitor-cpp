#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux.h"
#include "linux_parser.h"
#include "linux_process.h"
#include "linux_processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Linux::Linux() {
  // Kernel and Operating System doesn't change during the execution of system
  // monitor. So it can be initialized in constructor.
  kernel_ = LinuxParser::Kernel();
  operating_system_ = LinuxParser::OperatingSystem();
}

std::string Linux::Kernel() const { return kernel_; }

std::string Linux::OperatingSystem() const { return operating_system_; }

float Linux::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

int Linux::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int Linux::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int Linux::UpTime() const { return LinuxParser::UpTime(); }

LinuxProcessor &Linux::Cpu() { return cpu_; }

/*To return a vector of LinuxProcesses:
1. If process existe: Update calculation of Cpu Utilitization.
2. If process dosn't exist: Add new process to processes_ (with Cpu
Utilitization calculated)
3. Remove dead processes
4. Sort process by Cpu Utilization*/
vector<LinuxProcess> &Linux::Processes() {

  vector<int> pids = LinuxParser::Pids();

  for (int pid : pids) {
    const LinuxProcess &search_process{pid};
    auto it = std::find(
        processes_.begin(), processes_.end(),
        search_process); //(Using == operator overloaded in class LinuxProcess)
    if (it != processes_.end()) {
      // 1. Process exist
      LinuxProcess *pointer_process = processes_.data();
      // Accumulated for 10 seconds
      pointer_process[it - processes_.begin()].CalcCpuUtilization(10);
    } else {
      // 2. Process dosn't exist
      LinuxProcess new_process{pid};
      // First time, it is enough with 1 second accumulated.
      new_process.CalcCpuUtilization(1);
      processes_.emplace_back(new_process);
    }
  }

  // 3. Remove dead process
  for (long unsigned int i = 0; i < processes_.size(); i++) {
    if (std::find(pids.begin(), pids.end(), processes_[i].Pid()) ==
        pids.end()) {
      processes_.erase(processes_.begin() + i);
    }
  }

  // 4. Sort processes by CPU utilization. (Using < operator overloaded in class
  // LinuxProcess)
  std::sort(processes_.begin(), processes_.end());

  return processes_;
}