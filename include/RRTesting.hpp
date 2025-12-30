#ifndef RR_TESTING
#define RR_TESTING

#include "AcknowledgePacket.hpp"

/**
 * @brief Sends a test packet over the network
 *
 * @param seq The sequence number to track the test packets
 * @return True, if the seq does not exceed the testing limit
 */
bool sendTestPacket(uint8_t seq);

/**
 * @brief Marks the test packet as acknoledged
 *
 * @param ack_pkt The ACK packet
 */
void onTestAckReceived(AcknowledgePacket* ack_pkt);

/**
 * @brief Runs the tests
 */
void RRTest();

/**
 * @brief Kernel task to trigger the tests
 */
void RRTestTrigger();

#endif // !RR_TESTING
