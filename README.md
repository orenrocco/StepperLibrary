# StepperLibrary
C++
This library allows you to control unipolar or bipolar stepper motors.

example:
<code>
#include <Stepper.h>/n
    #define motorSteps 200/n     // change this depending on the number of steps
    #define motorPin1 8//IO/n
    #define motorPin2 9//IO/n
    #define ledPin 13
    //  initialize of the Stepper library:
    Stepper myStepper(motorSteps, motorPin1,motorPin2); 
    void motor() {
    //set the motor speed at 60 RPMS:
    myStepper.setSpeed(60);
    myStepper.step(100);
    delay(500);
    // Step backward 100 steps:
    Serial.println("Backward");
    myStepper.step(-100);
    delay(500); 
    }
    int main()
    {
    motor();
    }
</code>
