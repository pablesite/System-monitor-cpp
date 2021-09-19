#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "linux.h"
#include "linux_process.h"

namespace NCursesDisplay {
void Display(Linux &system, int n = 10);
void DisplaySystem(Linux &system, WINDOW *window);
void DisplayProcesses(std::vector<LinuxProcess> &processes, WINDOW *window,
                      int n);
std::string ProgressBar(float percent);
}; // namespace NCursesDisplay

#endif