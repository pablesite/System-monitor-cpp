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
    cpu_utilization_ = CpuUtilization();
}

int Process::Pid() { return pid_;}


float Process::CpuUtilization() { 

    // hay que devolver el acumulado de 10 segundos independientemente del refresco de la pantalla.
    // active_jiffies_acum = [1 2 3 4 5 6 7 8 9 10] --> [2 3 4 5 6 7 8 9 10 11] --> 
    
    // QUIZA UNA BUENA IDEA ES JUGAR CON LOS TIEPMOS EN CICLOS DE RELOJ PARA CONSEGUIR MÁS PRECISIÓN.... PERO LOS SEGUNDOS DEL PROCESADOR NO SE PUEDE...
    //std::cout << "(" << seconds_prev_ << ") ";
    long active_jiffies = LinuxParser::ActiveJiffies(pid_);
    //float total_time = LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK);//BORRAR
    long seconds = UpTime(); 


    ////std::cout << "" << pid_ << "-" << active_jiffies << "";
    
    //float cpu_utilization{0.0};
    //If seconds == 0, the process has just started and therefore cpu_utilization is 0.0.
    if(seconds >= 0) {
    
        long active_jiffies_d = active_jiffies - active_jiffies_prev_; 
        //float total_time_d = total_time - total_time_prev_; //BORRAR
        long seconds_d = seconds - seconds_prev_;

       //// std::cout << "(" << active_jiffies_d << " "<< active_jiffies << " " << active_jiffies_prev_ << ") ";
        if (seconds_d >= 3) { //check if this has any sense... i think so but...
            //std::cout << "totaltime:"<< total_time << " seconds:" << seconds << " tt_prev:" << total_time_prev_ << " s_prev:" << seconds_prev_ << " tt_d:" << total_time_d << " s_d:" << seconds_d<< "\n";
            std::cout << "(" << active_jiffies << ") ";
            active_jiffies_prev_ = active_jiffies;
            //setActiveJiffiesPrev(active_jiffies);
            //total_time_prev_ = total_time; //BORRAR
            seconds_prev_ = seconds;
            //std::cout << "(" << active_jiffies_prev_ << ")(" << active_jiffies << ")";
            //std::cout << "(" << &active_jiffies_prev_ << ") ";

            // // meto un int nuevo con valor 0
            // active_jiffies_acc_.emplace_back(0);
            // // añado active_jiffies_d a todos los int del vector.
            // for (long x : active_jiffies_acc_){
            //     x += active_jiffies_d;
            // }
            // // reverse del vector
            // std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());
            // // saco el último int con el acumulado de 10 veces. --> active_jiffies_d acum
            // long active_jiffies_final = active_jiffies_acc_.back();
            // //std::cout << "(" << active_jiffies_d << ") ";
            // if(active_jiffies_acc_.size() > 10){
            //     active_jiffies_acc_.pop_back();
            // }
            
            // // reverse del vector
            // std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());

            // similar para los seconds --> seconds_d acum

            cpu_utilization_ = (float)(active_jiffies_d/sysconf(_SC_CLK_TCK))/seconds_d;
            return cpu_utilization_;
            //return cpu_utilization = (float)(active_jiffies);
        }

    }
    return cpu_utilization_; 
}

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
    //std::cout << "utiliza: " << a.cpu_utilization_ << " "; // ver si es NAN aquí... porque esto hace que los procesos se desordenen un poco.
    return (a.cpu_utilization_ < this->cpu_utilization_) ?  true :  false;
}

void Process::setActiveJiffiesPrev (long active_jiffies_prev){
    active_jiffies_prev_ = active_jiffies_prev;
}

long Process::getActiveJiffiesPrev(){
    return active_jiffies_prev_;
}