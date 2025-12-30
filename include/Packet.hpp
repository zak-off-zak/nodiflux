#ifndef PACKET
#define PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>

// Packet Types
enum class PacketType : uint8_t {
  DIS = 0x01,
  ACK = 0x02,
  DAT = 0x03,
};

// Abstract Class for a Packet
/**
 * @class Packet
 * @brief Abstract class for the packet
 *
 * Other packet types are derived from that one.
 *
 */
class Packet {
  protected:
    PacketType type;
    uint8_t src[6];
    uint16_t pkt_id;
    uint8_t chs;
  public:
    /**
     * @brief Returns the type of the packet
     *
     * @return Type of the packet
     */
    PacketType getType() const { return this->type; }

    /**
     * @brief Returns the checksum of the packet
     *
     * @return Checksum of the packet
     */
    uint8_t getChecksum() const { return this->chs; }
    /**
     * @brief Return the ID of the packet
     *
     * @return ID of the packet
     */
    uint16_t getPacketId() const {return this->pkt_id; }

    /**
     * @brief Defaut destroctur
     */
    virtual ~Packet() = default;

    /**
     * @brief Seralizes the packet to a byte buffer.
     *
     * @param buffer Buffer to be filled with the packet bytes
     * @param buffer_size The max size of the buffer
     * @return The number of bytes written to the buffer
     */
    virtual size_t serialize(uint8_t* buffer, size_t buffer_size) const = 0;

    /**
     * @brief Deserializes the fields from the byte buffer into an packet object.
     *
     * @param buffer The byte buffer to deserialize from
     * @param len The length of the byte buffer
     * @return True, if the deserialization was successful
     */
    virtual bool deserializeFields(const uint8_t* buffer, size_t len) = 0;

    /**
     * @brief Calculates the simple XOR checksum over the fields of a packet
     *
     * @return One byte checksum of the packet
     */
    virtual uint8_t checksum() const = 0;

    /**
     * @brief Contains the halding logic for packets. Defines the way the procedures to be performed upon recieval of a packet.
     */
    virtual void handle() = 0;

    /**
     * @brief The factory to create different types of packets upon their recieval
     *
     * @param buffer Buffer to deserialize from
     * @param len Length of the buffer
     * @return Packet of a certain type
     */
    static Packet* deserializeFactory(const uint8_t* buffer, size_t len);
};



#endif // !PACKET
