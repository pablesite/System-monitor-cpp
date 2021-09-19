#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
  virtual int Pid() const = 0;
  virtual std::string User() const = 0;
  virtual std::string Command() const = 0;
  virtual float CpuUtilization() const = 0;

  virtual long int UpTime() const = 0;
  virtual std::string Ram() const = 0;

  virtual void CalcCpuUtilization(long unsigned int time_acc) = 0;

  /* I don't know how to make these functions virtual functions.
   They use a Process as input, and it would be necesseray pass a LinuxProcess
   when it is overriden.
   This produce an error despite LinuxProcess derive from Process */
  //   virtual bool operator<(Process const &a) const = 0;
  //   virtual bool operator==(Process const &a) const = 0;
};

#endif