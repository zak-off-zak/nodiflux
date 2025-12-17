#ifndef NODE_REGISTRY
#define NODE_REGISTRY

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <unordered_map>
#include <array>
#include <mutex>

struct MacHash{
  std::size_t operator()(const std::array<uint8_t, 6>& mac) const noexcept {
    std::size_t h = 0;
    for(uint8_t b : mac){
      h = (h << 5) ^ b ^ (h >> 2);
    }
    return h;
  }
};

class NodeRegistry{
  private:
    std::unordered_map<std::array<uint8_t, 6>, time_t, MacHash> nodes;
    mutable std::mutex mtx;
  public:
    NodeRegistry () = default;
    NodeRegistry(const NodeRegistry&) = delete;
    NodeRegistry& operator=(const NodeRegistry&) = delete;

    static NodeRegistry& instance() {
        static NodeRegistry instance;
        return instance;
    }

    void updateNode(const uint8_t mac[6], time_t t);
    std::array<uint8_t, 6> getMostRecentNode() const;
    bool peerExists(const uint8_t mac[6]);
    void debugMostRecentNode();
    void debug() const;
};


#endif // !NODE_REGISTRY
