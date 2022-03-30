#include <Joystick.h>

#define BUTTONS 4

struct status
{
  int axis[2], butons[BUTTONS];
} status[2];

union pins
{
  byte _pins[8];
  struct
  {
    struct
    {
      byte l, r;
    } axis[2];
    byte butons[BUTTONS];
  } pins;
} ctls[2];

Joystick_ joysticks[2] = {
    Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_MULTI_AXIS, BUTTONS, 0,
              true, true, false, false, false, false, false, false, false, false, false),
    Joystick_(JOYSTICK_DEFAULT_REPORT_ID + 10, JOYSTICK_TYPE_GAMEPAD, BUTTONS, 0,
              true, true, false, false, false, false, false, false, false, false, false)};

const int pinToButtonMap = 2;

byte selectPin = 15;
byte selectVals[4]{LOW, HIGH, HIGH, LOW};

void setup()
{
  ctls[0].pins = {{{2, 3}, {4, 5}}, {9, 9, 8, 8}};
  ctls[1].pins = {{{21, 20}, {19, 20}}, {10, 10, 16, 16}};

  pinMode(selectPin, OUTPUT);
  for (int ctl = 0; ctl < 2; ctl++)
  {
    for (int index = 0; index < (int)sizeof(ctls[ctl].pins); index++)
    {
      pinMode(ctls[ctl]._pins[index], INPUT_PULLUP);
    }
    joysticks[ctl].begin();
  }
}

void loop()
{
  for (int ctl = 0; ctl < 2; ctl++)
  {
    digitalWrite(selectPin, selectVals[HIGH]);
    for (int axis = 0; axis < 2; axis++)
    {
      int read = 512;
      read += 512 * !digitalRead(ctls[ctl].pins.axis[axis].l);
      read -= 512 * !digitalRead(ctls[ctl].pins.axis[axis].r);
      if (status[ctl].axis[axis] != read)
      {
        if (axis == 0)
        {
          joysticks[ctl].setXAxis(read);
        }
        else
        {
          joysticks[ctl].setYAxis(read);
        }
        status[ctl].axis[axis] = read;
      }
    }

    for (int index = 0; index < BUTTONS; index++)
    {
      digitalWrite(selectPin, selectVals[index]);
      int read = !digitalRead(ctls[ctl].pins.butons[index]);
      if (status[ctl].butons[index] != read)
      {
        joysticks[ctl].setButton(index, read);
        status[ctl].butons[index] = read;
      }
    }
  }

  delay(50);
}