#ifndef DISCOVERY_PACKET
#define DISCOVERY_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "Packet.hpp"


class DiscoveryPacket : public Packet {
  public:
    DiscoveryPacket();
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() override;
};

#endif // !DISCOVERY_PACKET
