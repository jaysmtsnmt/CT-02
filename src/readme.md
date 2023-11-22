USAGE PYTHON

[WRITE]
When writing to the Arduino, the format must be followed:
tag/function/data-->>
or
tag/function

Tags:
Starting with zeros (eg: 01), are reserved for writing. 
Starting with ones (eg: 11), are reserved for reading sensors. 

eyes: 01
heart: 02
rgb: 03
servo: 04
lightsensor: 11

Function:
Functions are defined in the classes in hardware.cpp. Check the file for class and function info. 

[READ]
When reading from the Arduino, a run function must be callled.

The results will be printed in the following format:
!/tag/data -->>

Results will also be returned from the function as 'returndata' or through the inheritance of the function:
get(inputdata, returndata);
int data = get(inputdata, returndata);

The tag is the hardware tag. (starting with 1_)

[SERVO]
Calling servo functions.

asa = ______________ <--- function
Exception: Detach Function

[LOGGING]
When changing states:
state = "idle";

You MUST declare log1 = loopnumber;

[STATES]
idle(<bool whether should fall asleep (default true)>, <int number of idle loops till sleep (default value is found in main.cpp)>);
sleep(<bool whether should wake after predetermined loops (default false)>, <int number of sleep loops till idle (default value found in main.cpp)>);
