# ChalGameOfLife

challenge: the game of life;

# requirement

- use Qt framework (versions 5 or 6)
- be multithreaded

## design

this is a simulation of micro-organisms (units);

the start: define the amount of units from 1 to 20;

each unit:
- has own properties: lifetime and "fertile frequency";
- lives in a separate thread (the main thread has to be independent);
- reproduces itself after it's fertile frequency, inheriting the temporary parent properties;
- dies after it's lifetime;

the main thread:
- allows using anti-biotics to kill a random amount of units;
- renders everything in runtime
- outputs the statistics about each unit

visible statistics:
- how long until death;
- how long until breed;
