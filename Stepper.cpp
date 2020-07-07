#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include "gpiolib.h"
#include "gpiolib.c"
#include "Stepper.h"

Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2)
{
gpio_export(motor_pin_1);
    gpio_export(motor_pin_2);
  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // time stamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;

  gpio_direction(this->motor_pin_1, 1);
  gpio_direction(this->motor_pin_2, 1);

  // When there are only 2 pins, set the others to 0:
  this->motor_pin_3 = 0;
  this->motor_pin_4 = 0;
  this->motor_pin_5 = 0;

  // pin_count is used by the stepMotor() method:
  this->pin_count = 2;
}


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                      int motor_pin_3, int motor_pin_4)
{
gpio_export(motor_pin_1);
    gpio_export(motor_pin_2);

  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // time stamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  gpio_direction(this->motor_pin_1, 1);
  gpio_direction(this->motor_pin_2, 1);
  gpio_direction(this->motor_pin_3, 1);
  gpio_direction(this->motor_pin_4, 1);

  // When there are 4 pins, set the others to 0:
  this->motor_pin_5 = 0;

  // pin_count is used by the stepMotor() method:
  this->pin_count = 4;
}

/*
 *   constructor for five phase motor with five wires
 *   Sets which wires should control the motor.
 */
Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                      int motor_pin_3, int motor_pin_4,
                                      int motor_pin_5)
{
gpio_export(motor_pin_1);
    gpio_export(motor_pin_2);
  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // time stamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;
  this->motor_pin_5 = motor_pin_5;

  // setup the pins on the microcontroller:
  gpio_direction(this->motor_pin_1, 1);
  gpio_direction(this->motor_pin_2, 1);
  gpio_direction(this->motor_pin_3, 1);
  gpio_direction(this->motor_pin_4, 1);
  gpio_direction(this->motor_pin_5, 1);

  // pin_count is used by the stepMotor() method:
  this->pin_count = 5;
}

/*
 * Sets the speed in revs per minute
 */
void Stepper::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */

uint64_t micros()
{
    uint64_t us = 
std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
                  now().time_since_epoch()).count();
    return us; 
}

void Stepper::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take
auto start = std::chrono::high_resolution_clock::now();
auto elapsed = std::chrono::high_resolution_clock::now() - start;
  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      if (this->pin_count == 5)
        stepMotor(this->step_number % 10);
      else
        stepMotor(this->step_number % 4);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
  if (this->pin_count == 2) {
    switch (thisStep) {
      case 0:  // 01
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
      break;
      case 1:  // 11
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 1);
      break;
      case 2:  // 10
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
      break;
      case 3:  // 00
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 0);
      break;
    }
  }
  if (this->pin_count == 4) {
    switch (thisStep) {
      case 0:  // 1010
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
      break;
      case 1:  // 0110
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
      break;
      case 2:  //0101
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
      break;
      case 3:  //1001
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
      break;
    }
  }

  if (this->pin_count == 5) {
    switch (thisStep) {
      case 0:  // 01101
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
        gpio_write(motor_pin_5, 1);
        break;
      case 1:  // 01001
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 0);
        gpio_write(motor_pin_5, 1);
        break;
      case 2:  // 01011
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
        gpio_write(motor_pin_5, 1);
        break;
      case 3:  // 01010
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
        gpio_write(motor_pin_5, 0);
        break;
      case 4:  // 11010
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 1);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
        gpio_write(motor_pin_5, 0);
        break;
      case 5:  // 10010
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 0);
        gpio_write(motor_pin_4, 1);
        gpio_write(motor_pin_5, 0);
        break;
      case 6:  // 10110
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 1);
        gpio_write(motor_pin_5, 0);
        break;
      case 7:  // 10100
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
        gpio_write(motor_pin_5, 0);
        break;
      case 8:  // 10101
        gpio_write(motor_pin_1, 1);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
        gpio_write(motor_pin_5, 1);
        break;
      case 9:  // 00101
        gpio_write(motor_pin_1, 0);
        gpio_write(motor_pin_2, 0);
        gpio_write(motor_pin_3, 1);
        gpio_write(motor_pin_4, 0);
        gpio_write(motor_pin_5, 1);
        break;
    }
  }
}

/*
  version() returns the version of the library:
*/
int Stepper::version(void)
{
  return 5;
}
