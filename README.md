# Synchronizacja-watkow
Description:

    Program implements football stadium, where fans are threads, guards, gates and checkouts are critical sections. To enter the stadium each fan must have ticket. Then he must acquire both one guard and one gate. After that he cheers for given amount of time (sleepTime), then he leaves the stadium(which also requires to access one gate) and cycle begins again. At the start of program tickets are assigned randomly, so some fans will have them at the first round of entrance. Every ticket expires after 10 entries.
Compilation: 

    make

Running program: 

    ./main noFans sleepTime noGates(optional)

Instruction: Status of the fans, gates and guars will automatically refresh

    Program ends when key "q" pressed.
