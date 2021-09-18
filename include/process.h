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
  float CpuUtilizationCalc();                   
  float CpuUtilization();                  
  std::string Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const; 
  bool operator==(const Process& a) const ;
//   bool operator!=(const Process& a) const ;  

  void setActiveJiffiesPrev(long active_jiffies_prev);
  long getActiveJiffiesPrev();

  // TODO: Declare any necessary private members
 private:
    int pid_;
    std::string user_;
    long active_jiffies_prev_{0};     
    long seconds_prev_{0};       
    float cpu_utilization_{0};
    std::vector<long> active_jiffies_acc_;
};

#endif