#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "linux_process.h"

using std::string;
using std::to_string;
using std::vector;

LinuxProcess::LinuxProcess(int pid) : pid_{pid} {
  // PID and USER doesn't change during the execution of system monitor. So it
  // can be initialized in constructor.
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  // Active Jiffies Prev and Seconds Prev should be established in the
  // construct in order to not obtain long term data in the first few seconds.
  aj_prev_ = LinuxParser::ActiveJiffies(pid);
  secs_prev_ = UpTime();
}

int LinuxProcess::Pid() const { return pid_; }

string LinuxProcess::User() const { return user_; }

string LinuxProcess::Command() const { return command_; }

float LinuxProcess::CpuUtilization() const { return cpu_utilization_; }

long int LinuxProcess::UpTime() const {
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
}

string LinuxProcess::Ram() const { return LinuxParser::Ram(pid_); }

/* Calculation of Cpu Utilization for each process is based on an accumulated of
 "time_acc" seconds. In each iteration the cpu utilization in last
 second is calculated. The accumulated is stored in a vector of "time_acc"
 positions adding this differential utilization each time.*/
void LinuxProcess::CalcCpuUtilization(long unsigned int time_acc) {
  // actual variables
  long aj_act = LinuxParser::ActiveJiffies(pid_);
  long secs_act = UpTime();
  // differential variables
  long aj_d{0};
  long secs_d{0};
  // final variables
  long aj{0};
  long secs{0};

  // Check if process has started.
  if (secs_act >= 0) {
    aj_d = aj_act - aj_prev_;
    secs_d = secs_act - secs_prev_;

    // Check if process is active in last iteration.
    if (secs_d >= 1) {
      aj_prev_ = aj_act;
      secs_prev_ = secs_act;

      /***** To improve calculation precission *****/
      // Introduce a new item in vectors to store the accumulated data.
      aj_acc_.emplace_back(0);
      secs_acc_.emplace_back(0);
      // Adding Active Jiffies Differential and Seconds Differential to all the
      // items of the vectors Active Jiffies Accumulated and Seconds Acumulated
      long *pointer_process = aj_acc_.data();
      for (vector<long>::iterator it = aj_acc_.begin(); it != aj_acc_.end();
           ++it) {
        pointer_process[it - aj_acc_.begin()] += aj_d;
      }
      long *pointer_seconds = secs_acc_.data();
      for (vector<long>::iterator it = secs_acc_.begin(); it != secs_acc_.end();
           ++it) {
        pointer_seconds[it - secs_acc_.begin()] += secs_d;
      }
      // Reverse of the vectors in order to have the maximum data accumulated in
      // the last element of the vectors
      std::reverse(aj_acc_.begin(), aj_acc_.end());
      std::reverse(secs_acc_.begin(), secs_acc_.end());
      // Obtain accumulated values.
      aj = aj_acc_.back();
      secs = secs_acc_.back();
      // Delete this accumulated values if size of vectors has reached its
      // limit.
      if (aj_acc_.size() > time_acc) {
        aj_acc_.pop_back();
      }
      if (secs_acc_.size() > time_acc) {
        secs_acc_.pop_back();
      }
      // Reverse of the vectors to have the minimum accumulated in last vector
      // to the next iteration.
      std::reverse(aj_acc_.begin(), aj_acc_.end());
      std::reverse(secs_acc_.begin(), secs_acc_.end());
      /***** End of improving the cpu utilization precission *****/

      cpu_utilization_ = ((float)(aj) / sysconf(_SC_CLK_TCK)) / secs;
    }
  }
}

bool LinuxProcess::operator<(LinuxProcess const &a) const {
  return (a.cpu_utilization_ < cpu_utilization_);
}

bool LinuxProcess::operator==(LinuxProcess const &a) const {
  return (a.pid_ == pid_);
}
