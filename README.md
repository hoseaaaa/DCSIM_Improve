A tool for large-scale power grid simulation, including DC and transient simulation. It will incorporate advanced technologies like distributed computing, hardware-software co-design, and approximate computing in the future.

Env Set:

    export DCSIM_PATH=$HOME/DCSIM_Improve
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib

Build:
    mkdir bin; 
    make;

Run (release):
    cd bin; 
    ./cqpgsim netlist-file(IBM format) solution-file;

Run (debug):
    cd bin; 
    ./cqpgsim netlist-file(IBM format) solution-file ref-solution-file;

...

This project is developed by Qian Chen (E-mail: chenqian0103@seu.edu.cn). The used sparse linear solver is developed by Xiaoming Chen.

Base :Qian Chen
Change : Yang Hz
