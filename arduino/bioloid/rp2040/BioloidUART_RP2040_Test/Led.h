/****************************************************************************
 *
 *   Copyright (c) 2024 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   Alternatively, this software may be distributed under the terms of BSD
 *   license.
 *
 *   See README and COPYING for more details.
 *
 ****************************************************************************/
/**
 *
 *   @file   Led.h
 *
 *   @brief  Abstracts an LED.
 *
 ****************************************************************************/

#pragma once

#include <Arduino.h>

//! Class for encapsulating an LED.
//! @details In particular, this class introduces the notion of on and off, Turning
//!          the LED on may require driving the pin LOW or HIGH depending on how it's
//!          wired.
class Led {
 public:
    //! Indicates how the LED is wired.
    enum class Active {
        LOW = 0,   //!< A LOW signal causes the LED to turn off.
        HIGH = 1,  //!< A HIGH signal causes the LedLED to turn on.
    };

    //! Constructor
    Led(pin_size_t pin,               //!< [in] Pin connected to the LED.
        Active active = Active::HIGH  //!< [in] Indicates how the LED is wired.
        )
        : pin{pin},
          on_val{active == Active::HIGH ? PinStatus::HIGH : PinStatus::LOW},
          off_val(active == Active::HIGH ? PinStatus::LOW : PinStatus::HIGH) {}

    //! Initializes the pin to be an output.
    void init() { pinMode(this->pin, OUTPUT); }

    //! Turns the LED on.
    void on() { digitalWrite(this->pin, this->on_val); }

    //! Turns the LED off.
    void off() { digitalWrite(this->pin, this->off_val); }

    //! Toggles the state of the LED.
    void toggle() {
        if (digitalRead(this->pin) == this->on_val) {
            digitalWrite(this->pin, this->off_val);
        } else {
            digitalWrite(this->pin, this->on_val);
        }
    }

 private:
    pin_size_t pin;     //!< This is the pin hooked up to the LED
    PinStatus on_val;   //!< This value turns the LED on
    PinStatus off_val;  //!< This value turns the LED off
};
