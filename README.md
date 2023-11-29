A tool for large-scale power grid simulation, including DC and transient simulation. It will incorporate advanced technologies like distributed computing, hardware-software co-design, and approximate computing in the future.
Data:
    ibm powergrid 
    thu powergrid 

Env Set:

    export DCSIM_HOME=$HOME/DCSIM_Improve
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DCSIM_HOME/lib

Build:
    make all 
    
Run (debug):
    make ibmpg1
    ... 
    make thupg1
    ...
    
...

This project is developed by Qian Chen . Change by Yanghaoze. The used sparse linear solver is developed by Xiaoming Chen.

Base :Qian Chen
Change : Yang Hz
