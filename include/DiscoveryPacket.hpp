#ifndef DISCOVERY_PACKET
#define DISCOVERY_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "Packet.hpp"


/**
 * @class DiscoveryPacket
 * @brief Derived class for discoverty packets.
 *
 * Discovery Packets are used to announce the existance of a packet on the network.
 *
 */
class DiscoveryPacket : public Packet {
  public:

    /**
     * @brief Creates an empty DIS packet. Meant to be filled by the deserializeFields function in the factrory.
     */
    DiscoveryPacket();

    /**
     * @brief Seralizes the DIS packet to a byte buffer.
     *
     * @param buffer Buffer to be filled with the DAT packet bytes
     * @param buffer_size The max size of the buffer
     * @return The number of bytes written to the buffer
     */
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;

    /**
     * @brief Deserializes the fields from the byte buffer into an DIS packet object.
     *
     * @param buffer The byte buffer to deserialize from
     * @param len The length of the byte buffer
     * @return True, if the deserialization was successful
     */
    bool deserializeFields(const uint8_t* buffer, size_t len) override;

    /**
     * @brief Calculates the simple XOR checksum over the fields of an DIS packet
     *
     * @return One byte checksum of the ACK packet
     */
    uint8_t checksum() const override;

    /**
     * @brief Contains the halding logic for DIS packets. Defines the way the procedures to be performed upon recieval of a packet.
     */
    void handle() override;
};

#endif // !DISCOVERY_PACKET
