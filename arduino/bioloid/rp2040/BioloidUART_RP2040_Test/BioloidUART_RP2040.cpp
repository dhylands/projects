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
 *   @file   BioloidUART_RP2040.cpp
 *
 *   @brief  Implements bioloid serial style operations whcih work under
 *           Arduino on an RP2040.
 *
 ****************************************************************************/

#include "BioloidUART_RP2040.h"

static uart_hw_t* get_uart_hw_for_serial_uart(SerialUART* serial_uart) {
    if (serial_uart == &Serial1) {
        return uart0_hw;
    }
    if (serial_uart == &Serial2) {
        return uart1_hw;
    }

    assert(!"Unknown serial port");
    return nullptr;
}

BioloidUART::BioloidUART(
    SerialUART* serial_uart,
    uint32_t baudRate,
    pin_size_t txPin,
    pin_size_t rxPin,
    size_t rxBufLen)
    : serial_uart{serial_uart},
      uart_hw{get_uart_hw_for_serial_uart(serial_uart)},
      baudRate{baudRate},
      txPin{txPin},
      rxPin{rxPin},
      rxBufLen{rxBufLen} {}

void BioloidUART::begin() {
    this->serial_uart->setTX(this->txPin);
    this->serial_uart->setRX(this->rxPin);
    this->serial_uart->setFIFOSize(this->rxBufLen);
    this->serial_uart->begin(this->baudRate);

    // Enable the Receiver and disable the Transmitter
    uint32_t cr = this->uart_hw->cr;
    cr |= UARTCR_RXE;
    cr &= ~UARTCR_TXE;
    this->uart_hw->cr = cr;
}

void BioloidUART::write_packet(size_t numBytes, const uint8_t* data) {
    uart_hw_t* uart_regs = this->uart_hw;

    // Disable Rx and enable Tx
    uint32_t cr = uart_regs->cr;
    cr &= ~UARTCR_RXE;
    cr |= UARTCR_TXE;
    uart_regs->cr = cr;

    // Write the data
    this->serial_uart->write(data, numBytes);

    // Wait for the data to be sent
    while ((uart_regs->fr & UARTFR_DONE_MASK) != UARTFR_DONE) {
    }

    // Enable the Receiver and disable the Transmitter
    cr |= UARTCR_RXE;
    cr &= ~UARTCR_TXE;
    uart_regs->cr = cr;
}
