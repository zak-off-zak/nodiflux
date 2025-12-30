#ifndef NODE_REGISTRY
#define NODE_REGISTRY

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <unordered_map>
#include <array>
#include <mutex>

/**
 * @class MacHash
 * @brief Struct used to calculte the hash of a MAC address for it to be able to be stored in an unordered map as a key
 *
 */
struct MacHash{
  std::size_t operator()(const std::array<uint8_t, 6>& mac) const noexcept {
    std::size_t h = 0;
    for(uint8_t b : mac){
      h = (h << 5) ^ b ^ (h >> 2);
    }
    return h;
  }
};

/**
 * @class NodeRegistry
 * @brief A singelton to keep trak of the nodes, that were seen on by a given node.
 *
 * Once a DIS packet is recieved an entry to the regostry containing the MAC of the sender and the current time is made.
 *
 */
class NodeRegistry{
  private:
    std::unordered_map<std::array<uint8_t, 6>, time_t, MacHash> nodes;
    mutable std::mutex mtx;
  public:

    /**
     * @brief Default constructor
     */
    NodeRegistry () = default;

    /**
     * @brief Delete the copy constructor
     */
    NodeRegistry(const NodeRegistry&) = delete;

    /**
     * @brief Detele copy assignment operator
     *
     * @return NodeRegistry
     */
    NodeRegistry& operator=(const NodeRegistry&) = delete;

    /**
     * @brief Method to get the singelton instance of the NodeRegistry
     *
     * @return Singelton instance of the NodeRegistry
     */
    static NodeRegistry& instance() {
        static NodeRegistry instance;
        return instance;
    }

    /**
     * @brief Add or update the node
     *
     * @param mac MAC address of the node
     * @param t Current time
     */
    void updateNode(const uint8_t mac[6], time_t t);

    /**
     * @brief Get the most recent node from the regitry
     */
    std::array<uint8_t, 6> getMostRecentNode() const;
    /**
     * @brief Check if the peer is in the registry
     *
     * @param mac MAC adress of the peer
     * @return True, if the peer is in the registry
     */
    bool peerExists(const uint8_t mac[6]);
};


#endif // !NODE_REGISTRY
