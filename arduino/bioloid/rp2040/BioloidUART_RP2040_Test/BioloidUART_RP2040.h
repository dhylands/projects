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
 *   @file   BioloidUART_RP2040.h
 *
 *   @brief  Implements bioloid serial style operations whcih work under
 *           Arduino on an RP2040.
 *
 ****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <Arduino.h>

#include <cassert>

//! Implements the logic to support using the bioloid bus.
class BioloidUART {
 public:
    //! Constructor
    BioloidUART(
        SerialUART* serial_uart,  //!< [in] Serial UART to use for underlying comms
        uint32_t baudRate,        //!< [in] Baud rate to use.
        pin_size_t txPin,         //!< [in] Tx PIN to use
        pin_size_t rxPin,         //!< [in] Rx pin to use
        size_t rxBufLen = 64      //!< [in] Size of the Rx buffer
    );

    //! Initialize the bioloid uart.
    void begin();

    //! Returns the number of bytes available for reading.
    int available() { return this->serial_uart->available(); }

    //! Reads a byte from the UART.
    uint8_t read_byte() { return this->serial_uart->read(); }

    //! Writes a packet to the bioloid bus.
    void write_packet(
        size_t numBytes,     //!< [in] Number of bytes to write.
        const uint8_t* data  //!< [in] Data to write.
    );

 private:
    static constexpr uint32_t UARTCR_RXE = 1 << 9;  //!< Receiver Enable.
    static constexpr uint32_t UARTCR_TXE = 1 << 8;  //!< Transmitter Enable.

    static constexpr uint32_t UARTFR_TXFE = 1 << 7;  //!< Transmit FIFO Empty.
    static constexpr uint32_t UARTFR_TXFF = 1 << 5;  //!< Transmit FIFO Full.
    static constexpr uint32_t UARTFR_BUSY = 1 << 3;  //!< Busy transmitting.

    static constexpr uint32_t UARTFR_DONE_MASK = UARTFR_TXFE | UARTFR_BUSY;  //!< Done xmit mask
    static constexpr uint32_t UARTFR_DONE = UARTFR_TXFE;  //!< Done xmit condition

    SerialUART* const serial_uart;  //!< Serial UART we're using.
    uart_hw_t* const uart_hw;       //!< Pointer to UART registers.
    uint32_t baudRate;              //!< Initial baud rate to use.
    pin_size_t txPin;               //!< Tx Pin to use.
    pin_size_t rxPin;               //!< Rx Pin to use.
    size_t rxBufLen;                //!< Size of Rx Buffer.
};
