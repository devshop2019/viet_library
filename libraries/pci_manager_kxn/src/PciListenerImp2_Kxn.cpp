/**
 * File: PciListenerImp2_Kxn.cpp
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
#include "PciListenerImp2_Kxn.h"
#include <IPciChangeHandler_Kxn.h>

PciListenerImp2_Kxn::PciListenerImp2_Kxn() {
}

void PciListenerImp2_Kxn::init(byte pin, IPciChangeHandler_Kxn* handler, bool pullUp) {
  this->pciPin = pin;
  this->_pciChangeHandler = handler;
  this->io_Bit = NK_PIN_TO_BITMASK(pin);
  this->io_Reg = NK_PIN_TO_BASEREG(pin);
  
  if(pullUp) {
//    pinMode(pin, INPUT_PULLUP);
    NK_DIRECT_MODE_INPUT(this->io_Reg, this->io_Bit);
    NK_DIRECT_MODE_PULLUP(this->io_Reg, pin);
  } else {
//    pinMode(pin, INPUT);
    NK_DIRECT_MODE_INPUT(this->io_Reg, this->io_Bit);
  }

  this->lastVal = NK_DIRECT_READ(this->io_Reg, this->io_Bit);
}

void PciListenerImp2_Kxn::pciHandleInterrupt(byte vect) {
  byte val = NK_DIRECT_READ(this->io_Reg, this->io_Bit);
  if(val != this->lastVal) {
    this->lastVal = val;
    this->_pciChangeHandler->pciHandleChange(val, this);
  }
}
