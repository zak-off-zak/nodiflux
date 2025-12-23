#include "RRTesting.hpp"
#include "AcknowledgePacket.hpp"
#include "DataPacket.hpp"
#include "HardwareSerial.h"
#include "Protocol.hpp"
#include "Tasks.hpp"

static const uint8_t TEST_MAC[] = {0x20, 0xE7, 0xC8, 0x5A, 0x2D, 0x18};
static const uint8_t TEST_COUNT = 100;
static const uint32_t TEST_TIMEOUT_MS = 2000;

static uint32_t sent = 0;
static uint32_t received = 0;
static uint32_t rttSum = 0;

static uint32_t sendTime[TEST_COUNT];
static bool acked[TEST_COUNT];

bool sendTestPacket(uint8_t seq) {
    if (seq >= TEST_COUNT) return false;

    DataPacket pkt(std::to_string(seq), TEST_MAC);
    sendTime[seq] = millis();
    acked[seq] = false;

    sendPacket(TEST_MAC, pkt);
    sent++;
    return true;
}

void onTestAckReceived(AcknowledgePacket* ack_pkt) {
    if (!ack_pkt) return;

    uint16_t seq = ack_pkt->getAckPacketId();
    Serial.println(seq);

    if (seq >= TEST_COUNT) return;
    if (acked[seq]) return;

    uint32_t rtt = millis() - sendTime[seq];
    rttSum += rtt;
    received++;
    acked[seq] = true;
}

void RRTest() {
    Serial.println("========= RUNNING RR TEST =========");

    sent = 0;
    received = 0;
    rttSum = 0;
    memset(acked, 0, sizeof(acked));

    for (uint8_t i = 0; i < TEST_COUNT; i++) {
        sendTestPacket(i);
        delay(100);
    }

    uint32_t startWait = millis();
    while (millis() - startWait < TEST_TIMEOUT_MS) {
        delay(1);
    }

    float packetLoss =
        sent > 0 ? 100.0f * (sent - received) / sent : 0.0f;

    float avgRtt =
        received > 0 ? (float)rttSum / received : 0.0f;

    Serial.println("========= RR TEST RESULTS =========");
    Serial.print("Sent: "); Serial.println(sent);
    Serial.print("Received: "); Serial.println(received);
    Serial.print("Packet loss: ");
    Serial.print(packetLoss); Serial.println(" %");
    Serial.print("Average RTT: ");
    Serial.print(avgRtt); Serial.println(" ms");
    Serial.println("===================================");
}


void RRTestTrigger(){
    static String cmd;

    while (true) {
        while (Serial.available()) {
            char c = Serial.read();

            if (c == '\n' || c == '\r') {
                cmd.trim();

                if (cmd == "rrtest") {
                    Serial.println("[CMD] Starting RR test");

                    xTaskCreate(
                        rrTestTask,
                        "RRTest",
                        4096,
                        nullptr,
                        2,
                        nullptr
                    );
                }

                cmd = "";
            } else {
                cmd += c;
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

