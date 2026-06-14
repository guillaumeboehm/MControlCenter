#ifndef CLI_H
#define CLI_H

#include "operate.h"
#include "options.h"

class CLI {
  public:
    CLI() = delete;
    explicit CLI(Operate &&operate, Options &&options);

    int execute();

  private:
    void setCoolerBooster(Options::CoolerBoostState);
    void setUserMode(Options::UserMode);

    Operate operate;
    Options options;
};

#endif // !CLI_H
