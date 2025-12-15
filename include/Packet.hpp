#ifndef PACKET
#define PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "config.hpp"

// Packet Types
enum class PacketType : uint8_t {
  DIS = 0x01,
  ACK = 0x02,
  DAT = 0x03,
};

// Abstract Class for a Packet
class Packet {
  protected:
    PacketType type;
    uint8_t src[6];
    uint16_t pkt_id;
    uint8_t chs;
  public:
    PacketType getType() const { return this->type; }
    uint8_t getChecksum() const { return this->chs; }

    virtual ~Packet() = default;
    virtual size_t serialize(uint8_t* buffer, size_t buffer_size) const = 0;
    virtual bool deserializeFields(const uint8_t* buffer, size_t len) = 0;
    virtual uint8_t checksum() const = 0;
    virtual void handle() const = 0;

    static Packet* deserializeFactory(const uint8_t* buffer, size_t len);
};

class DataPacket : public Packet {
  protected:
    uint8_t dest[6];
    uint8_t ttl;
    uint8_t msg[DATA_MESSAGE_SIZE];
  public:

    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() const override;
};

class DiscoveryPacket : public Packet {
  public:
    DiscoveryPacket();
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() const override;
};

class AcknowledgePacket : public Packet {
  protected:
    uint8_t dest[6];
    uint8_t ttl;
  public:
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;
    bool deserializeFields(const uint8_t* buffer, size_t len) override;
    uint8_t checksum() const override;
    void handle() const override;
};

#endif // !PACKET
