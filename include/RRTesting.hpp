#ifndef RR_TESTING
#define RR_TESTING

#include "AcknowledgePacket.hpp"

bool sendTestPacket();
void onTestAckReceived(AcknowledgePacket* ack_pkt);
void RRTest();
void RRTestTrigger();

#endif // !RR_TESTING
