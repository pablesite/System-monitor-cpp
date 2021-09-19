#ifndef LINUX_PROCESSOR_H
#define LINUX_PROCESSOR_H

#include "processor.h"

class LinuxProcessor : public Processor {
public:
  float Utilization() override;

private:
  /*****To be used to calculate cpu utilization*****/
  long idle_j_prev_{0};
  long active_j_prev_{0};
};

#endif