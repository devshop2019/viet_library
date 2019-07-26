/**
 * File: PciListenerImp_Kxn.h
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

#ifndef PCI__H_LISTENERIMP_H_
#define PCI__H_LISTENERIMP_H_

#include <Arduino.h>
#include <PciListener_Kxn.h>
#include "arduino_io.h"

#define CHANGEKIND_HIGH_TO_LOW LOW
#define CHANGEKIND_LOW_TO_HIGH HIGH

/**
 * A basic PinChangeInterrupt listener implementation.
 */
class PciListenerImp_Kxn : public PciListener_Kxn
{
  public:
    /**
     * For the constructor add a function callback, that will be called either with CHANGEKIND_HIGH_TO_LOW or CHANGEKIND_LOW_TO_HIGH.
     */
    PciListenerImp_Kxn(byte pin, void (*callback)(byte changeKind), bool pullUp = false);
    virtual void pciHandleInterrupt(byte vector);
  private:
    void (*_callback)(byte changeKind);
	byte _lastVal;
        NK_IO_REG_TYPE My_ssBit;
        volatile NK_IO_REG_TYPE *My_ssReg;
};

#endif

