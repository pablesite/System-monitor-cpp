#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                             
  std::string User();                     
  std::string Command();
  void CpuUtilizationCalc();                   
  float CpuUtilization();                  
  std::string Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const; 
  bool operator==(Process const& a) const ;


  // TODO: Declare any necessary private members
 private:
    int pid_;
    std::string user_;
    long active_jiffies_prev_{0};     
    long seconds_prev_{0};       
    float cpu_utilization_{0};
    std::vector<long> active_jiffies_acc_{10,0};
    std::vector<long> seconds_acc_{10,0};
    
};

#endif