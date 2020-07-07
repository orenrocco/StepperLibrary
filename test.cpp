#include <iostream>
#include "Stepper.h"
#include "Stepper.cpp"
using namespace std;

// change this to the number of steps on your motor
const int STEPS= 200;
Stepper stepper1(STEPS, 22,36);
int previous = 0; 
int val;
int temp=0;
void bull(){
stepper1.setSpeed(30);
while(true) {
  // get the sensor value
  cin>>val;
  // move a number of steps equal to the change in the
  temp=val-previous;
  stepper1.step(temp);
  // remember the previous value of the sensor
  previous = val;
}
}

int main() {
  bull();

}
