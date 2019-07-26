/**
 * File: PciManager.h
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

#ifndef PCI_H_MANAGER_H_
#define PCI_H_MANAGER_H_

#include "PciListener_Kxn.h"

/**
 * Register PCI listeners to pins with this Manager class.
 * The listener will be called on pin change of the vector of pins that this pin belongs to. So you always have
 * to check if your pin was changed or not. 
 */
class PciManagerClass_Kxn
{
  public:
    /**
     * Register a pin for pin change interrupt.
     *  pin - The pin the listener to be registered for pinChange.
     *  listener - The lister that is called on pin change. Note, that as matters stand, you souldn't register
     *    a listener multiply times (e.g. for more pins).
     */
    void registerListener(byte pin, PciListener_Kxn* listener);
    /**
     * Register a pin for pin change interrupt. Same as above but the pin needs to be configured in the listener.
     *  listener - The lister that is called on pin change. Note, that as matters stand, you souldn't register
     *    a listener multiply times (e.g. for more pins).
     */
    void registerListener(PciListener_Kxn* listener);
    /**
     * Remove registration of a pin for PCI.
     *  listener - The that should be removed.
     */
    void removeListener(PciListener_Kxn* listener);

    /**
     * For internal use only.
     */
    void callListeners(byte pciVectorId);
  private:
    void add(PciListener_Kxn* listener);
    void remove(PciListener_Kxn* listener);
    PciListener_Kxn* _firstListener;

    #if (defined(__SAM3X8E__) || defined(ARDUINO_ARCH_ESP8266))
        static void callListeners();
    #endif
};

extern PciManagerClass_Kxn PciManager_Kxn;

#endif
