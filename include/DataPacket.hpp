#ifndef DATA_PACKET
#define DATA_PACKET

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include "Packet.hpp"

#include "config.hpp"

/**
 * @class DataPacket
 * @brief Derived class specifing the layout and functionality of the DAT packet.
 *
 * The DAT packet is sent over the network to carry a meaninful payload in the msg field.
 *
 */
class DataPacket : public Packet {
  protected:
    uint8_t dest[6];
    uint8_t ttl;
    uint8_t msg[DATA_MESSAGE_SIZE];
  public:
    /**
     * @brief Defines the common routing procedures for both DAT and ACK packets on the network
     *
     * @tparam PktT Taype of the Packet
     * @tparam LambdaCallback Type of the callback function
     * @tparam AckCallBack Type of the callback function to be called on ACK
     * @param pkt The actual packet to be routed
     * @param lambdaCallback Callback funtion to execute during the routing procedure
     */
    template<typename PktT, typename LambdaCallback, typename AckCallBack>
    friend void commonRouting(PktT& pkt, LambdaCallback lambdaCallback, AckCallBack ackCallBack);

    /**
     * @brief Get the destination of a DAT packet
     *
     * @return A pointer to an array of 6 bytes containg the dest MAC address
     */
    const uint8_t* getDest() {return this->dest; }

    /**
     * @brief Constructor to create DAT packets from msg and dest
     *
     * @param message Message to be sent over the network
     * @param dest Destination node for the messag to be sent to
     */
    DataPacket(const std::string& message, const uint8_t dest[6]);

    /**
     * @brief Creates an empty DAT packet. Meant to be filled by the deserializeFields function in the factrory.
     */
    DataPacket();

    /**
     * @brief Seralizes the DAT packet to a byte buffer.
     *
     * @param buffer Buffer to be filled with the DAT packet bytes
     * @param buffer_size The max size of the buffer
     * @return The number of bytes written to the buffer
     */
    size_t serialize(uint8_t* buffer, size_t buffer_size) const override;

    /**
     * @brief Deserializes the Fields from the byte buffer into an DAT packet object.
     *
     * @param buffer The byte buffer to deserialize from
     * @param len The length of the byte buffer
     * @return True, if the deserialization was successful
     */
    bool deserializeFields(const uint8_t* buffer, size_t len) override;

    /**
     * @brief Calculates the simple XOR checksum over the fields of an DAT packet
     *
     * @return One byte checksum of the ACK packet
     */
    uint8_t checksum() const override;

    /**
     * @brief Contains the halding logic for DAT packets. Defines the way the procedures to be performed upon recieval of a packet.
     */
    void handle() override;
};

#endif // !DATA_PACKET
