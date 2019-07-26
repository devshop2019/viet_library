/**
 * File: PciListenerImp_Kxn.cpp
 * Description:
 * PciManager library is an easy to use Pin Change Interrupt manager for Arduino.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2012 Balazs Kelemen
 * Copying permission statement:
    This file is part of PciManager.

    PciManager is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Arduino.h"
#include "PciListenerImp_Kxn.h"

PciListenerImp_Kxn::PciListenerImp_Kxn(byte pin, void (*callback)(byte changeKind), bool pullUp) {
  this->pciPin = pin;
  this->_callback = callback;
  
    My_ssBit = NK_PIN_TO_BITMASK(pin);          // get send pin's ports and bitmask
    My_ssReg = NK_PIN_TO_BASEREG(pin);         // get pointer to output register
  if(pullUp) {
//    pinMode(pin, INPUT_PULLUP);
    NK_DIRECT_MODE_INPUT(My_ssReg, My_ssBit);
    NK_DIRECT_MODE_PULLUP(My_ssReg, pin);
  } else {
//    pinMode(pin, INPUT);
    NK_DIRECT_MODE_INPUT(My_ssReg, My_ssBit);
  }

//  this->_lastVal = digitalRead2(this->pciPin);
  this->_lastVal = NK_DIRECT_READ(My_ssReg, My_ssBit);

//  if(pullUp) {
//    pinMode(pin, INPUT_PULLUP);
//  } else {
//    pinMode(pin, INPUT);
//  }
//  this->_lastVal = digitalRead(this->pciPin);
}

void PciListenerImp_Kxn::pciHandleInterrupt(byte vect) {
//  byte val = digitalRead2(this->pciPin);
  byte val = NK_DIRECT_READ(My_ssReg, My_ssBit);
//  byte val = digitalRead(this->pciPin);
  if(val != this->_lastVal) {
    this->_lastVal = val;
    this->_callback(val);
  }
}

