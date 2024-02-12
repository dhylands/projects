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
 *   @file   LedSequenceh
 *
 *   @brief  Plays a pattern on a LED with minimal interaction.
 *
 ****************************************************************************/

#pragma once

#include <Arduino.h>

#include "Led.h"

//! Plays a sequence of flashes on a LED.
class LedSequence {
 public:
    //! Is the sequence continuous or one shot?
    enum class Mode {
        CONTINUUS,  //!< Sequence repeats
        ONE_SHOT,   //!< Sequence runs once and stops.
    };

    //! @brief Constructor
    LedSequence(
        Led* led,                    //!< [in] The LED to control.
        uint16_t* seq,               //!< [in] The sequence of time intervals
        size_t seq_len,              //!< [in] Length of the sequence.
        Mode mode = Mode::CONTINUUS  //!< [in] Type of sequence
        )
        : led{led}, seq{seq}, seq_len{seq_len}, mode{mode} {}

    //! Initializes the LED sequence.
    void init() {
        this->led->init();
        this->led->off();
        this->seq_idx = -1;
        this->lastToggle = 0;
        if (this->mode == Mode::CONTINUUS) {
            this->touch();
        }
    }

    //! Runs the sequence, as appropriate
    void run() {
        if (this->seq_idx < 0) {
            // Sequence hasn't been started yet
            return;
        }
        if (millis() - this->lastToggle < this->seq[this->seq_idx]) {
            // Not enough time has elapsed
            return;
        }

        this->led->toggle();
        this->lastToggle = millis();
        this->seq_idx++;
        if (this->seq_idx >= this->seq_len) {
            if (this->mode == Mode::CONTINUUS) {
                this->seq_idx = 0;
            } else {
                this->seq_idx = -1;
                this->led->off();
            }
        }
    }

    //! Starts the sequence if it isn't currently running.
    void touch() {
        if (this->seq_idx >= 0) {
            // Sequence already running - nothing to do
            return;
        }
        this->seq_idx = 0;
        this->led->on();
        this->lastToggle = millis();
    }

 private:
    Led* const led;       //!< The LED being controlled.
    uint16_t* const seq;  //!< The sequence being run.
    size_t seq_len;       //!< The length of the sequence.
    Mode mode;            //!< Is the sequence one-shot or continuous?

    uint32_t lastToggle = 0;  //!< Last time that the LED was toggled.
    ssize_t seq_idx = -1;     //!< Where we are in the sequence.
};
