#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>


class Process {
 public:
  Process(int pid);

  int Pid() const;                             
  std::string User() const;                      
  std::string Command() const;
  float CpuUtilization() const;   
                      
  long int UpTime() const;
  std::string Ram() const;  

  void CalcCpuUtilization(long unsigned int time_acc);                   
                 
  bool operator<(Process const& a) const; 
  bool operator==(Process const& a) const ;

 private:
    //private members to be returned througt members function
    int pid_;
    std::string user_;
    std::string command_;
    float cpu_utilization_;

    //private members to be used internally to calculate cpu utilization.
    long active_jiffies_prev_{0};     
    long seconds_prev_{0};       
    std::vector<long> active_jiffies_acc_{10,0};
    std::vector<long> seconds_acc_{10,0};
};

#endif