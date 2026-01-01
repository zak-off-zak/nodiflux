#ifndef ACKNOWLEDGE_PACKET
#define ACKNOWLEDGE_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "Packet.hpp"

/**
 * @class AcknowledgePacket
 * @brief The class inheriting from the Packet class. Defines additional fields specific for the ACK packtes on the network.
 *
 * This packe is sent as a response to a DAT packet. It contains the ack_pkt_id where the ID of the received DAT packet is saved.
 *
 */
class AcknowledgePacket : public Packet {
  protected:
    uint16_t ack_pkt_id;
    uint8_t dest[6];
    uint8_t ttl;
  public:

    /**
     * @brief Defines the common routing procedures for both DAT and ACK packets on the network
     *
     * @tparam PktT Taype of the Packet
     * @tparam LambdaCallback Type of the callback function
     * @param pkt The actual packet to be routed
     * @param lambdaCallback Callback funtion to execute during the routing procedure
     */
    template<typename PktT, typename LambdaCallback>
    friend void commonRouting(PktT& pkt, LambdaCallback lambdaCallback);

    /**
     * @brief Return the ack_pkt_id of an ACK packet
     *
     * @return ack_pkt_id of the packet
     */
    uint16_t getAckPacketId() {return this->ack_pkt_id; }

    /**
     * @brief Constructs the ACK packet to be sent to the dest with an ID of the received DAT packet.
     *
     * @param Dest MAC adress of the node to send the ACK to
     * @param packet_id The packet ID of the received DAT packet
     */
    AcknowledgePacket(const uint8_t dest[6], const uint16_t packet_id);

    /**
     * @brief Creates an empty ACK packet. Meant to be filled by the deserializeFields function in the factrory.
     */
    AcknowledgePacket();

    /**
     * @brief Seralizes the ACK packet to a byte buffer.
     *
     * @param buffer Buffer to be filled with the ACK packet bytes
     * @param buffer_size The max size of the buffer
     * @return The number of bytes written to the buffer
     */
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;

    /**
     * @brief Deserializes the Fields from the byte buffer into an ACK packet object.
     *
     * @param buffer The byte buffer to deserialize from
     * @param len The length of the byte buffer
     * @return True, if the deserialization was successful
     */
    bool deserializeFields(const uint8_t* buffer, size_t len) override;

    /**
     * @brief Calculates the simple XOR checksum over the fields of an ACK packet
     *
     * @return One byte checksum of the ACK packet
     */
    uint8_t checksum() const override;

    /**
     * @brief Contains the halding logic for ACK packets. Defines the way the procedures to be performed upon recieval of a packet.
     */
    void handle() override;
};

#endif // !ACKNOWLEDGE_PACKET
