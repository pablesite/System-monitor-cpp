#ifndef PROCESS_H
#define PROCESS_H

#include <string>
// #include <vector>

class Process {
public:
  //Process();
  //virtual ~Process() = default;
  virtual int Pid() const = 0;
  virtual std::string User() const = 0;
  virtual std::string Command() const = 0;
  virtual float CpuUtilization() const = 0;

  virtual long int UpTime() const = 0;
  virtual std::string Ram() const = 0;

  virtual void CalcCpuUtilization(long unsigned int time_acc) = 0;

//   virtual bool operator<(Process const &a) const = 0;
//   virtual bool operator==(Process const &a) const = 0;

};

#endif