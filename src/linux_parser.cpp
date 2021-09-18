#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream> //only to debug

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// To review. Is there another way of memory calculation better?
float LinuxParser::MemoryUtilization() { 
  // MemTotal - MemFree
  string line;
  string key;
  string value;
  float mem_total{0};
  float mem_free{0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = std::stoi(value);
        }
        if (key == "MemFree:") {
          mem_free = std::stoi(value);
        }
      }      
    }
  }
  return (mem_total - mem_free)/mem_total;
 }


/* The first value represents the total number of seconds the system has been up. 
The second value is the sum of how much time each core has spent idle, in seconds. 
Consequently, the second value may be greater than the overall system uptime on systems with multiple cores.*/
long LinuxParser::UpTime() { 
  string system_uptime = "0";
  string idle_uptime = "0";
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime >> idle_uptime;
  }
  return std::stol(system_uptime);
}


long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }


long LinuxParser::ActiveJiffies(int pid) { 
  // ActiveJiffies = utime_ + stime_ + cstime_ + cutime_.
  std::vector<std::string> process_u = LinuxParser::CpuUtilization(pid);                                
  return  std::stol(process_u[ProcessStates::utime_]) + 
          std::stol(process_u[ProcessStates::stime_]) +
          std::stol(process_u[ProcessStates::cstime_]) + 
          std::stol(process_u[ProcessStates::cutime_]); 
}


long LinuxParser::ActiveJiffies() { 
  // ActiveJiffies = user + nice + system + irq + softirq + steal.  
  std::vector<std::string> cpu_u = LinuxParser::CpuUtilization();                    
  return  std::stol(cpu_u[CPUStates::kUser_]) + 
          std::stol(cpu_u[CPUStates::kNice_]) + 
          std::stol(cpu_u[CPUStates::kSystem_]) + 
          std::stol(cpu_u[CPUStates::kIRQ_]) + 
          std::stol(cpu_u[CPUStates::kSoftIRQ_]) + 
          std::stol(cpu_u[CPUStates::kSteal_]);  
  }


long LinuxParser::IdleJiffies() { 
  // IdleJiffies = idle + iowait.
  std::vector<std::string> cpu_u = LinuxParser::CpuUtilization();                    
  return  std::stol(cpu_u[CPUStates::kIdle_]) + 
          std::stol(cpu_u[CPUStates::kIOwait_]); 
  }


vector<string> LinuxParser::CpuUtilization() { 
  std::vector<std::string> cpu_utilization;
  string line, key, kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, kSteal, kGuest, kGuestNice ;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> kIRQ >> kSoftIRQ >> kSteal >> kGuest >> kGuestNice) {
        if (key == "cpu") {
          cpu_utilization = {kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, kSteal, kGuest, kGuestNice};
          break;
        }
      }      
    }
  }
  return cpu_utilization;
 }

 vector<string> LinuxParser::CpuUtilization(int pid) {   
  std::vector<std::string> process_utilization = {"0", "0", "0", "0"}; //Me aseguro de que devuelvo valores convertibles.
  string line, utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);    
    for (int i = 1; i <= 14; i++){
      linestream >> utime;
    }
    linestream >> stime >> cutime >> cstime;
    process_utilization = {utime, stime, cutime, cstime};
  }
  return process_utilization;
 }


int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = std::stoi(value);
        }
      }      
    }
  }
  return processes;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int procs_running{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          procs_running = std::stoi(value);
        }
      }      
    }
  } 
  return procs_running;
}


string LinuxParser::Command(int pid) { 
  string line;
  string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command; 
  }  
  return command; 
}


string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string vmsize;
  std::ifstream filestream(kProcDirectory +  std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> vmsize) {
        if (key == "VmSize:") {
          vmsize = to_string(std::stol(vmsize)/1024);
          return vmsize;
          break;
        }
      }      
    }
  } 
  return vmsize;
}


string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string uid;
  std::ifstream filestream(kProcDirectory +  std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          return uid;
          break;
        }
      }      
    }
  } 
  return uid; 
}


string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  string user;
  string access_id;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '/', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> access_id >> value) {
        if (value == uid) {
          return user;
        }
      }
    }
  }
  return value;
}


// It could be interesting to check if linux kernel is under 2.6 in order to deal with Jiffies.
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;
  string process_uptime = "0";
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; i++ ){
      linestream >> process_uptime;
    }
  }

  //std::cout << "UPTIME: " << process_uptime << "\n";
  try{
        uptime = std::stol(process_uptime)/sysconf(_SC_CLK_TCK);
    }
       
    // catch invalid_argument exception.
    catch(const std::invalid_argument &){
        //cerr << "Invalid argument" << "\n";
        uptime = 0;
    }
  uptime = std::stol(process_uptime)/sysconf(_SC_CLK_TCK);

  return uptime;

}
