#ifndef DATA_PACKET
#define DATA_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include "Packet.hpp"

#include "config.hpp"

class DataPacket : public Packet {
  protected:
    uint8_t dest[6];
    uint8_t ttl;
    uint8_t msg[DATA_MESSAGE_SIZE];
  public:
    const uint8_t* getDest() {return this->dest; }

    DataPacket(const std::string& message, const uint8_t dest[6]);
    DataPacket();
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() override;
};

#endif // !DATA_PACKET
