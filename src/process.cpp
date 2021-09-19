#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// pid and user doesn't change during the execution of system monitor.
// pid dosn't need invariant and can be initialed directly.
Process::Process(int pid): pid_{pid} {
    user_ = LinuxParser::User(pid_);
    command_ = LinuxParser::Command(pid_);
    //CpuUtilizationCalc(this);
    //Inicializar active jiffies prev para un buen cálculo de la cpu desde el inicio
    active_jiffies_prev_ = LinuxParser::ActiveJiffies(pid);
    seconds_prev_ = UpTime();
}

int Process::Pid() const { return pid_;}

/* User of a process doesn't change while the process is runnig --> It might think that user_ could save as private members in Process class.
 However, in every iteration of ncurses_display the processes are instanciated in order to have the processes updated.
 So, the user can be parsed from LinuxParser directly*/
string Process::User() const { return user_; }

/* The same explanation as Process::User() */
string Process::Command() const { return command_; }

float Process::CpuUtilization() const { return cpu_utilization_; }


// Funciones que se actualizan siempre
long int Process::UpTime() const { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); } 

string Process::Ram() const { return LinuxParser::Ram(pid_); }


//time_acc: time accumulated in seconds for each processor.
void Process::CalcCpuUtilization(long unsigned int  time_acc) {
    // hay que devolver el acumulado de 10 segundos independientemente del refresco de la pantalla.
    // active_jiffies_acum = [1 2 3 4 5 6 7 8 9 10] --> [2 3 4 5 6 7 8 9 10 11] --> 
   
    long active_jiffies = LinuxParser::ActiveJiffies(pid_);
    long seconds = UpTime();
    long active_jiffies_d{0};
    long seconds_d{0};
    long active_jiffies_final{0};
    long seconds_final{0};
    

    if(seconds >= 0) {
        active_jiffies_d = active_jiffies - active_jiffies_prev_; 
        seconds_d = seconds - seconds_prev_;

  
        if (seconds_d >= 1) { //Esto será necesario...?
            
            active_jiffies_prev_ = active_jiffies;
            seconds_prev_ = seconds;

        
            // Aumentar precisión de cálculo
            active_jiffies_acc_.emplace_back(0);
            seconds_acc_.emplace_back(0);

            // añado active_jiffies_d a todos los int del vector.
            long *pointer_process = active_jiffies_acc_.data();
            for (vector<long>::iterator it = active_jiffies_acc_.begin(); it!=active_jiffies_acc_.end(); ++it) {

                pointer_process[it - active_jiffies_acc_.begin()] += active_jiffies_d;
            }
            long *pointer_seconds= seconds_acc_.data();
            for (vector<long>::iterator it = seconds_acc_.begin(); it!=seconds_acc_.end(); ++it) {
                pointer_seconds[it - seconds_acc_.begin()] += seconds_d;
            }

            // reverse del vector
            std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());
            std::reverse(seconds_acc_.begin(), seconds_acc_.end());
            // saco el último int con el acumulado de 10 veces. --> active_jiffies_d acum
            active_jiffies_final = active_jiffies_acc_.back();
            seconds_final = seconds_acc_.back();
          
            
            if(active_jiffies_acc_.size() > time_acc){
                active_jiffies_acc_.pop_back();
            }

            if(seconds_acc_.size() > time_acc){
                seconds_acc_.pop_back();
            }
            // reverse del vector
            std::reverse(active_jiffies_acc_.begin(), active_jiffies_acc_.end());
            std::reverse(seconds_acc_.begin(), seconds_acc_.end());
                       
            cpu_utilization_= ((float)(active_jiffies_final)/sysconf(_SC_CLK_TCK))/seconds_final;        
         } 
    }
}


//Funciones de operator overload para procesos.
bool Process::operator<(Process const& a) const { return (a.cpu_utilization_ < cpu_utilization_); }

bool Process::operator==(Process const& a) const  { return (pid_ == a.pid_); }

