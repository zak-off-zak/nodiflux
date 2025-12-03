#include <cstdint>
#include <iostream>
#include "Packet.hpp"


int main() {
  //Testing Packet
  
  // Creating a packet
  Packet pkt{};
  pkt.src = 1;
  pkt.dest = 2;
  pkt.type = PacketType::DAT;
  pkt.ttl = 11;
  pkt.pkt_id = 1;
  for(int i = 0; i < 20; i++){
    pkt.msg[i] = i;
  }
  pkt.chs =   checksum(pkt);

  std::cout << "Checksum: " << std::hex << int(pkt.chs) << std::endl;
  
  // Serializing
  uint8_t buffer[sizeof(Packet)] = {0};
  serialize(pkt, buffer);
  std::cout << "Buffer: " << std::endl;
  for(int i = 0; i < sizeof(Packet); i++){
    std::cout << std::hex << int(buffer[i]) << " ";
  }
  std::cout << std::dec << std::endl;

  // Deserialize
  Packet recieved{};
  bool valid = deserialize(buffer, recieved);
  std::cout << "Packet validity: " << valid << std::endl;
  std::cout << "src = " << recieved.src << std::endl << "dest = " << recieved.dest << std::endl;

  return 0;
}
