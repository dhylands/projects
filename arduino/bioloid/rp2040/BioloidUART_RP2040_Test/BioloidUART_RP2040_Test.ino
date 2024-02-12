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
 *   @file   BioloidUART_RP2040_Test.ino
 *
 *   @brief  Test program for the BioloidUART_RP2040.h/.cpp files.
 *
 ****************************************************************************/

#include <Arduino.h>
#include <stdint.h>

#include "BioloidPacket.h"
#include "BioloidUART_RP2040.h"
#include "DumpMem.h"
#include "Led.h"
#include "LedSequence.h"

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(LED_BUILTIN)
Led led{LED_BUILTIN, Led::Active::HIGH};

static uint16_t heartbeat_seq[] = {100, 100, 100, 700};
static LedSequence heartbeat{&led, heartbeat_seq, LEN(heartbeat_seq), LedSequence::Mode::CONTINUUS};
#endif

static uint8_t packet1_params[16];
static uint8_t packet2_params[16];

static Bioloid::Packet packet1(packet1_params, LEN(packet1_params));
static Bioloid::Packet packet2(packet2_params, LEN(packet2_params));

static bool debug = false;
static uint8_t packet1_buf[32];  // Used for debug
static uint8_t packet2_buf[32];  // Used for debug

BioloidUART bioloid_uart1{&Serial1, 1000000, 12, 13};
BioloidUART bioloid_uart2{&Serial2, 1000000, 8, 9};

static uint8_t sendId = 1;

void setup() {
#if defined(LED_BUILTIN)
    heartbeat.init();
#endif

    Serial.begin();

    bioloid_uart1.begin();
    bioloid_uart2.begin();
}

static void process_cmd(uint8_t ch) {
    switch (ch) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            sendId = ch - '0';
            Serial.printf("Set ID to %u\n", sendId);
            break;
        }

        case 'd': {
            debug = !debug;
            Serial.printf("Toggle DEBUG %s\n", debug ? "ON" : "OFF");
            break;
        }
        case 'p': {
            Serial.printf("Sending PING to ID %u\n", sendId);

            uint8_t cmd[6];
            cmd[0] = 0xff;
            cmd[1] = 0xff;
            cmd[2] = sendId;
            cmd[3] = 2;
            cmd[4] = Bioloid::as_uint8_t(Bioloid::Command::PING);
            cmd[5] = ~(cmd[2] + cmd[3] + cmd[4]);

            if (debug) {
                DumpMem("1 W", 0, cmd, LEN(cmd));
            }
            bioloid_uart1.write_packet(LEN(cmd), cmd);
            break;
        }
    }
}

// Bus 1 acts like a master
static void process_bus1(uint8_t byte) {
    auto err = packet1.ProcessChar(byte);
    if (err == Bioloid::Error::NOT_DONE) {
        return;
    }
    auto cmd = packet1.Command();
    uint8_t id = Bioloid::as_uint8_t(packet1.ID());
    if (debug) {
        auto err = packet1.ErrorCode();
        Serial.printf("Received Error %s from ID %" PRIu8 "\n", Bioloid::as_str(err), id);
        size_t pktLen = packet1.Data(LEN(packet1_buf), packet1_buf);
        DumpMem("1 R", 0, packet1_buf, pktLen);
    }
}

// Bus 2 acts like a device
static void process_bus2(uint8_t byte) {
    auto err = packet2.ProcessChar(byte);
    if (err == Bioloid::Error::NOT_DONE) {
        return;
    }
    auto cmd = packet2.Command();
    uint8_t id = Bioloid::as_uint8_t(packet2.ID());
    if (debug) {
        Serial.printf("Received %s for ID %" PRIu8 "\n", Bioloid::as_str(cmd), id);
        size_t pktLen = packet2.Data(LEN(packet2_buf), packet2_buf);
        DumpMem("2 R", 0, packet2_buf, pktLen);
    }
    if (cmd == Bioloid::Command::PING) {
        Serial.printf("Got a PING\n");

        uint8_t rsp[6];

        rsp[0] = 0xff;
        rsp[1] = 0xff;
        rsp[2] = id;
        rsp[3] = 2;  // For length and status
        rsp[4] = Bioloid::as_uint8_t(Bioloid::Error::NONE);
        rsp[5] = ~(rsp[2] + rsp[3] + rsp[4]);

        if (debug) {
            Serial.printf("Sending PING Response\n");
            DumpMem("2 W", 0, rsp, LEN(rsp));
        }
        bioloid_uart2.write_packet(sizeof(rsp), rsp);
    }
}

void loop() {
#if defined(LED_BUILTIN)
    heartbeat.run();
#endif

    if (Serial.available()) {
        int ch = Serial.read();
        process_cmd(ch);
    }

    if (bioloid_uart1.available()) {
        uint8_t byte = bioloid_uart1.read_byte();
        process_bus1(byte);
    }

    if (bioloid_uart2.available()) {
        uint8_t byte = bioloid_uart2.read_byte();
        process_bus2(byte);
    }
}
