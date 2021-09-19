#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux.h"
#include "linux_processor.h"
#include "linux_process.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Linux::Linux() {
  // Characteristic that doesn't change during program execution.
  kernel_ = LinuxParser::Kernel();
  operating_system_ = LinuxParser::OperatingSystem();
}

// Const functions.
std::string Linux::Kernel() const { return kernel_; }

std::string Linux::OperatingSystem() const { return operating_system_; }

// Parser functions.
float Linux::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

int Linux::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int Linux::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int Linux::UpTime() const { return LinuxParser::UpTime(); }

// Componentes. (otras clases)
LinuxProcessor &Linux::Cpu() { return cpu_; }

vector<LinuxProcess> &Linux::Processes() {

  vector<int> pids{};
  // Nuevos procesos o actualizo los que existen
  for (int pid : LinuxParser::Pids()) {
    pids.emplace_back(pid);

    const LinuxProcess &search_process{pid};
    auto it = std::find(processes_.begin(), processes_.end(), search_process);
    if (it != processes_.end()) {
      // Proceso ya existe
      LinuxProcess *pointer_process = processes_.data();
      pointer_process[it - processes_.begin()].CalcCpuUtilization(10);

    } else {
      // Nuevo proceso
      LinuxProcess new_process{pid};
      new_process.CalcCpuUtilization(
          1); // time_acc debería ser para toda la clase...?
      processes_.emplace_back(new_process);
    }
  }

  // Elimino procesos obsoletos
  for (long unsigned int i = 0; i < processes_.size(); i++) {
    if (std::find(pids.begin(), pids.end(), processes_[i].Pid()) ==
        pids.end()) {
      // Si el proceso no se corresponde con ningún pid lo elimino.
      processes_.erase(processes_.begin() + i);
    }
  }

  // order process by CPU utilization.
  std::sort(
      processes_.begin(),
      processes_.end()); // This use the < operator implemented un process.cpp

  return processes_;
}

// LinuxProcess &Linux::Test() {
//   LinuxProcess myprocess{1};
//   return myprocess;
// }