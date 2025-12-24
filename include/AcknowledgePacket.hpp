#ifndef ACKNOWLEDGE_PACKET
#define ACKNOWLEDGE_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "Packet.hpp"

class AcknowledgePacket : public Packet {
  protected:
    uint16_t ack_pkt_id;
    uint8_t dest[6];
    uint8_t ttl;
  public:
    uint16_t getAckPacketId() {return this->ack_pkt_id; }
    AcknowledgePacket(const uint8_t dest[6], const uint16_t packet_id);
    AcknowledgePacket();
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() override;
};

#endif // !ACKNOWLEDGE_PACKET
