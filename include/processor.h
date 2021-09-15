#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 

 private:
    long idle_j_prev_{0};
    long active_j_prev_{0};
};

#endif