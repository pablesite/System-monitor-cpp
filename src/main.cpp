#include "ncurses_display.h"
#include "system.h"

//Only for debug
#include <chrono>
#include <string>
#include <thread>
#include <iostream>

int main() {
  System system;
  NCursesDisplay::Display(system);
  
  // std::vector<Process> processes;
  // while(1){
  //   processes = system.Processes();
  //   std::cout << "\n\nProcesses: Nº " << processes.size() << ": ";

  //   for (int i = 0; i < 10; ++i) {
  //     float cpu = processes[i].CpuUtilization() * 100; 
  //     //std::cout << "(" << cpu << ") ";
  //   }
  //   system.SortProcesses();

  //   std::cout << "Final" << "\n";

  //   std::this_thread::sleep_for(std::chrono::seconds(1));
  // }
}