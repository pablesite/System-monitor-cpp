#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {

/***** PATHS *****/
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};


/***** SYSTEM *****/
std::string OperatingSystem();
std::string Kernel();
float MemoryUtilization();
long UpTime();
int TotalProcesses();
int RunningProcesses();
std::vector<int> Pids();


/***** CPU *****/
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<std::string> CpuUtilization();
long ActiveJiffies();
long IdleJiffies();
long Jiffies();


/***** PROCESSES *****/
enum ProcessStates {
  utime_ = 0,
  stime_,
  cutime_,
  cstime_,
};

std::string Uid(int pid);
std::string User(int pid);
std::vector<std::string> CpuUtilization(int pid);
long ActiveJiffies(int pid);
std::string Ram(int pid);
long int UpTime(int pid);
std::string Command(int pid);
};  // namespace LinuxParser

#endif