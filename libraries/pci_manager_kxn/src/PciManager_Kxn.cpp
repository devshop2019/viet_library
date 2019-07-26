/**
 * File: PciManager_Kxn.cpp
 * Description:
 * PciManager_Kxn library is an easy to use Pin Change Interrupt manager for Arduino.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2017 Balazs Kelemen
 * Copying permission statement:
    This file is part of PciManager_Kxn.

    PciManager_Kxn is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    @ Note:
    @ Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used:
        10, 11, 12, 13, 14, 15,
        50, 51, 52, 53,
        A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
    @ Not all pins on the Leonardo and Micro support change interrupts, so only the following can be used:
        8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
*/

#include "Arduino.h"
#include "PciManager_Kxn.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>

/**
 * Add listener to the chain.
 */
void PciManagerClass_Kxn::add(PciListener_Kxn* listener) {

  // -- A listener should be registered only once.
  this->remove(listener);
  
  if(this->_firstListener == NULL) {
  
    // -- This is the first listener being registered.
    this->_firstListener = listener;
    
  } else {
  
    PciListener_Kxn* lastlistener = this->_firstListener;
    // -- Find the last listener, to build a chain.
    while(lastlistener->pciNextListener != NULL) {
      lastlistener = lastlistener->pciNextListener;
    }
    // -- Last listener found, let's add this listener to the end of the chain.
    lastlistener->pciNextListener = listener;
    
  }
  listener->pciNextListener = NULL;
}


/**
 * Remove listener from the chain.
 */
void PciManagerClass_Kxn::remove(PciListener_Kxn* listener) {
  if(this->_firstListener != NULL) {
    if(this->_firstListener == listener) {
      // -- This was the first listener.
      this->_firstListener = listener->pciNextListener;
    } else {
      PciListener_Kxn* lastlistener = this->_firstListener;
      // -- Find this listener in the chain.
      while(lastlistener->pciNextListener != NULL) {
        if(lastlistener->pciNextListener == listener) {
          // -- Remove the listener with joining the chain.
          lastlistener->pciNextListener = listener->pciNextListener;
          break;
        }
        lastlistener = lastlistener->pciNextListener;
      }
    }
  }
}

void PciManagerClass_Kxn::registerListener(byte pin, PciListener_Kxn* listener) {
  // -- PCINT vector will be 0, 1 or 2 depending on the pin.
  // --  digitalPinToPCICRbit(pin) will calculate which one it is.

    listener->pciPin = pin;

#if (defined(__SAM3X8E__) || defined(ARDUINO_ARCH_ESP8266))
    attachInterrupt(digitalPinToInterrupt(pin), this->callListeners, CHANGE);

#elif defined(digitalPinToPCICRbit)
  listener->pciVector = digitalPinToPCICRbit(pin);
  volatile uint8_t* pcicr = digitalPinToPCICR(pin);
  *pcicr |= (1 << listener->pciVector);
  volatile uint8_t* pcmsk = digitalPinToPCMSK(pin);
  *pcmsk |= (1 << digitalPinToPCMSKbit(pin));
#endif

  this->add(listener);
}

void PciManagerClass_Kxn::registerListener(PciListener_Kxn* listener) {
  this->registerListener(listener->pciPin, listener);
}

void PciManagerClass_Kxn::removeListener(PciListener_Kxn* listenerToRemove) {
    byte temPin = listenerToRemove->pciPin;

#if (defined(__SAM3X8E__) || defined(ARDUINO_ARCH_ESP8266))
    detachInterrupt(temPin);
#endif
  this->remove(listenerToRemove);
  boolean hasMoreListenersOnSamePin = false;
  boolean hasMoreListenersOnSameVector = false;

  PciListener_Kxn* listener = _firstListener;
  while(listener != NULL) {
    if(listener->pciPin == listenerToRemove->pciPin) {
      hasMoreListenersOnSamePin = true;
      hasMoreListenersOnSameVector = true;
      break;
    }
    if(listener->pciVector == listenerToRemove->pciVector) {
      hasMoreListenersOnSameVector = true;
    }
    listener = listener->pciNextListener;
  }
  
  if(!hasMoreListenersOnSamePin) {
    // -- Remove mask if no other uses this pin.

#if defined(digitalPinToPCICRbit)
    volatile uint8_t* pcmsk = digitalPinToPCMSK(listenerToRemove->pciPin);
    *pcmsk &= ~(1 << digitalPinToPCMSKbit(listenerToRemove->pciPin));

    if(!hasMoreListenersOnSameVector) {
      // -- Remove vector registration if no listeners for this vector.
      volatile uint8_t* pcicr = digitalPinToPCICR(listenerToRemove->pciPin);
      *pcicr &= ~(1 << listenerToRemove->pciVector);
    }
#endif
  }
}

/**
 * Walk through the chain and call all listener.
 */
void PciManagerClass_Kxn::callListeners(byte pciVectorId) {
  // -- (If first is NULL, than nothing is registered.)
  PciListener_Kxn* listener = _firstListener;
  while(listener != NULL) {
    if(listener->pciVector == pciVectorId) {
      listener->pciHandleInterrupt(pciVectorId);
    }
    listener = listener->pciNextListener;
  }
}

#if (defined(__SAM3X8E__) || defined(ARDUINO_ARCH_ESP8266))
    void PciManagerClass_Kxn::callListeners(){
        PciManager_Kxn.callListeners(0);
    }
#endif

/**
 * Global interrupt handling implementations.
 */
#if defined(PCINT0_vect)
ISR(PCINT0_vect){
  PciManager_Kxn.callListeners(0);
}
#endif
#if defined(PCINT1_vect)
ISR(PCINT1_vect){
  PciManager_Kxn.callListeners(1);
}
#endif
#if defined(PCINT2_vect)
ISR(PCINT2_vect){
  PciManager_Kxn.callListeners(2);
}
#endif

/**
 * Create a singleton from this manager class.
 */
PciManagerClass_Kxn PciManager_Kxn;
