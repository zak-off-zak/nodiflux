#ifndef PROTOCOL
#define PROTOCOL

#include <cstdint>
#include <esp_now.h>
#include "Packet.hpp"
#include "RetryJournal.hpp"
#include "NodeRegistry.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief The callback funtion to be called once any data is recieved
 *
 * @param mac_addr MAC address of the sender
 * @param incomingData Incoming data from the sender (Pointer!)
 * @param len The length of the incoming data
 */
void onDataReceive(const uint8_t* mac_addr, const uint8_t *incomingData, int len);

/**
 * @brief The callback to be executed once the data is sent over ESP-NOW
 *
 * @param mac_addr The MAC address of the recipient
 * @param status The status of the data delivery
 */
void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);

/**
 * @brief Funtion to send any kind of packet of the network
 *
 * @param mac_addr The pointer to the MAC address of the recipient
 * @param packet The packet to send
 */
void sendPacket(const uint8_t* mac_addr, const Packet& packet);

/**
 * @brief funtion to send out a discovery packet (broadcast)
 *
 * @param broadcastAddress The pointer to the broadcast address
 */
void sendDiscoveryPacket(const uint8_t* broadcastAddress);

/**
 * @brief The debug function to send a data packet from the serial interface
 *
 * @param dest The destionation of the packet to be sent to
 */
void sendDataPacket(const uint8_t* dest);

/**
 * @brief The fucntion to establish a peer connection with a node on the network
 *
 * @param mac_addr The pointer to the MAC address of the peer
 * @param channel [TODO:parameter]
 * @param encrypt [TODO:parameter]
 */
void establishPeer(const uint8_t* mac_addr, uint8_t channel, bool encrypt);

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
void commonRouting(PktT& pkt, LambdaCallback lambdaCallback, AckCallBack ackCallBack){
  pkt.ttl--;
  pkt.chs = pkt.checksum();

  if(pkt.ttl <= 0){
    Serial.println("TTL <= 0 -> Dropping the packet");
    return;
  }

  uint8_t mac_bytes[6];
  macStringToBytes(WiFi.macAddress(), mac_bytes);
  if(isMACEqual(pkt.dest, mac_bytes)){
    lambdaCallback();
  } else {
    if(pkt.getType() == PacketType::ACK){
      ackCallBack();
    }
    if(NodeRegistry::instance().peerExists(pkt.dest)){
      sendPacket(pkt.dest, pkt);
    }else{
      sendPacket(NodeRegistry::instance().getMostRecentNode().data(), pkt);
    }
  }

}

#endif // !PROTOCOL
