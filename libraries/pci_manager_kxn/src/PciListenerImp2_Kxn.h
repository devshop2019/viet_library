/**
 * File: PciListenerImp2_Kxn.h
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

#ifndef PCI_H_LISTENERIMP2_KXN_H_
#define PCI_H_LISTENERIMP2_KXN_H_

#include <Arduino.h>
#include <PciListener_Kxn.h>

class IPciChangeHandler_Kxn;

/**
 * An extended PinChangeInterrupt listener implementation.
 * The difference between this, and PciListener_KxnImpl, is that this also passes the listener instance to the handler on event.
 */
class PciListenerImp2_Kxn : public PciListener_Kxn
{
  public:
    PciListenerImp2_Kxn();
    /**
     * Initialize the listener.
     *  - pin - Pin to listen
     *  - handler - Handler
     *  - puppUp - Set true, to have the pin set in pullup mode.
     */
    void init(byte pin, IPciChangeHandler_Kxn* handler, bool pullUp = false);
    virtual void pciHandleInterrupt(byte vector);
    byte lastVal;
  private:
    IPciChangeHandler_Kxn* _pciChangeHandler;
};

#endif

