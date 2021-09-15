#include <string>

#include "format.h"

using std::string;


string Format::OutputFormat(int unit_time){
    if(unit_time < 10){
        return "0" + std::to_string(unit_time);
    } else {
        return std::to_string(unit_time);
    }
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

//This function repeat too much times... to review (check if it is better to use Format::Time())
string Format::ElapsedTime(long secs) { 
    int hours = (int)secs/3600;
    int minutes = (int)(secs/60)%60;
    int seconds = (int)secs%60;

    return OutputFormat(hours) + ":" + OutputFormat(minutes) + ":" + OutputFormat(seconds); 
}