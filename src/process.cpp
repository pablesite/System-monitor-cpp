#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

#include "linux_parser.h"
#include "system.h"
#include <iostream>

using std::string;
using std::to_string;
using std::vector;

// pid and user doesn't change during the execution of system monitor.
// pid dosn't need invariant and can be initialed directly.
Process::Process(int pid): pid_{pid} {
    //CpuUtilizationCalc(this);
}

int Process::Pid() { return pid_;}

float Process::CpuUtilizationCalc() {
    // hay que devolver el acumulado de 10 segundos independientemente del refresco de la pantalla.
    // active_jiffies_acum = [1 2 3 4 5 6 7 8 9 10] --> [2 3 4 5 6 7 8 9 10 11] --> 
   
    long active_jiffies = LinuxParser::ActiveJiffies(pid_);
    long seconds = UpTime();
    long active_jiffies_d{0};
    long seconds_d{0};
    

    if(seconds >= 0) {
        active_jiffies_d = active_jiffies - active_jiffies_prev_; 
        seconds_d = seconds - seconds_prev_;

        //std::cout << "(" << active_jiffies_prev_ << ")";
        if (seconds_d >= 3) {
            
            active_jiffies_prev_ = active_jiffies;
            seconds_prev_ = seconds;

                //         // // meto un int nuevo con valor 0
    //         // active_jiffies_acc_.emplace_back(0);
    //         // // añado active_jiffies_d a todos los int del vector.
    //         // for (long x : active_jiffies_acc_){
    //         //     x += active_jiffies_d;
    //         // }
    //         // // reverse del vector
    //         // std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());
    //         // // saco el último int con el acumulado de 10 veces. --> active_jiffies_d acum
    //         // long active_jiffies_final = active_jiffies_acc_.back();
    //         // //std::cout << "(" << active_jiffies_d << ") ";
    //         // if(active_jiffies_acc_.size() > 10){
    //         //     active_jiffies_acc_.pop_back();
    //         // }
            
    //         // // reverse del vector
    //         // std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());

    //         // similar para los seconds --> seconds_d acum
    //         
            //std::cout << "(" << active_jiffies_d << ") ";

            // if(pid_ == 3679){
            //     std::cout << "(seconds_d: " << seconds_d << ") ";
            //     std::cout << "(seconds: " << seconds << ") ";
            //     std::cout << "(seconds_prev_: " << seconds_prev_ << ")\n ";
            // }
            // if(pid_ == 3679){
            //     std::cout << "(active_jiffies_d: " << active_jiffies_d << ") ";
            //     std::cout << "(active_jiffies: " << active_jiffies << ") ";
            //     std::cout << "(active_jiffies_prev_: " << active_jiffies_prev_ << ") ";
            //     std::cout << "(CPU : " << (float)((float)(active_jiffies_d)/seconds_d) << ")\n ";
            // }

                       
            cpu_utilization_= ((float)(active_jiffies_d)/sysconf(_SC_CLK_TCK))/seconds_d;        

         } 
      
    }

    return cpu_utilization_;
}

float Process::CpuUtilization() { return cpu_utilization_; }

/* The same explanation as Process::User() */
string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

/* User of a process doesn't change while the process is runnig --> It might think that user_ could save as private members in Process class.
 However, in every iteration of ncurses_display the processes are instanciated in order to have the processes updated.
 So, the user can be parsed from LinuxParser directly*/
string Process::User() { return LinuxParser::User(pid_); }


long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); } 

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    //std::cout << "(" << a.cpu_utilization_ << "-" << cpu_utilization_ << ") "; // ver si es NAN aquí... porque esto hace que los procesos se desordenen un poco.
    //std::cout << a.cpu_utilization_ << "[]";
    return (a.cpu_utilization_ < cpu_utilization_) ?  true :  false;
}

bool Process::operator==(const Process &a) const  { 
    return (pid_ == a.pid_);
}



// bool Process::operator!=(const Process &a) const  { 
//     return (Pid() != a.Pid());
// }

// bool operator==( const Book &an ) const { 
//     return ISDN() == an.ISDN(); 
//     }

void Process::setActiveJiffiesPrev (long active_jiffies_prev){
    active_jiffies_prev_ = active_jiffies_prev;
}

long Process::getActiveJiffiesPrev(){
    return active_jiffies_prev_;
}