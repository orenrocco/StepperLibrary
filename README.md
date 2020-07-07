# StepperLibrary
C++
This library allows you to control unipolar or bipolar stepper motors.

example:


```c++
#include "Stepper.h"     
#include <iostream>
#define motorSteps 200    // change this depending on the number of steps
#define motorPin1 8//GPIO
#define motorPin2 9//GPIO
#define ledPin 13
//  initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1,motorPin2); 
void motor() {
    //set the motor speed at 60 RPMS:
    myStepper.setSpeed(60);
    myStepper.step(100);
    sleep(500)
    // Step backward 100 steps:
    myStepper.step(-100);
    sleep(500)
    }
    int main()
    {
    motor();
    }
    

