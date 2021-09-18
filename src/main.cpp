#include "ncurses_display.h"
#include <vector>
#include "system.h"
#include "process.h"

//Only for debug
#include <chrono>
#include <string>
#include <thread>
#include <iostream>

int main() {
  std::vector<Process > processes = {};
  // Process myprocess{1};
  // processes.emplace_back(myprocess);
  //System system(processes);
  System system;
  NCursesDisplay::Display(system, 10);
  // std::vector<Process* > &proceses_return =  system.Processes();
  // std::cout << "Size of processes_return: " << proceses_return.size() << "\n";
  // std::cout << "Pid of last process: " << proceses_return.back()->Pid() << "\n";



}