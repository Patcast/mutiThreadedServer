# Multithreaded  and multiprocess Server

This is an implementation of a server that can connect with multiple temperature sensors simultaniously and complete a variaty of actins with the data provided by the sensors. 

### Funtionalities
- Allows TCP connections with multiple sensors
- Data received form sensors is stored in a SQL db
- Alerts: Main process communicates events to a child process that writes log messages in a log file. This events can be when the temperature is above or below a threshold. 


