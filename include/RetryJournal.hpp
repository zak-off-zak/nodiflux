#ifndef RETRY_JOURNAL
#define RETRY_JOURNAL

#include "DataPacket.hpp"
#include <cstdint>
#include <ctime>
#include <memory>
#include <mutex>
#include <unordered_map>

/**
 * @class RetryJournalEntry
 * @brief The structure of an entry inside of the RetryJournal
 *
 */
struct RetryJournalEntry{
  std::unique_ptr<DataPacket> pkt;
  time_t timestamp;
  uint8_t retries;
};

/**
 * @class RetryJournal
 * @brief Singelton object to track the packets that were sent out and perform retries
 *
 */
class RetryJournal {
  private:
    std::unordered_map<uint16_t, std::unique_ptr<RetryJournalEntry>> entries;
    mutable std::mutex mtx;
  public:

    /**
     * @brief Default constructor
     */
    RetryJournal () = default;

    /**
     * @brief Delete the copy constructor
     */
    RetryJournal(const RetryJournal&) = delete;

    /**
     * @brief Detele copy assignment operator
     *
     * @return RetryJournal
     */
    RetryJournal& operator=(const RetryJournal&) = delete;

    /**
     * @brief Method to get the singelton instance of the RetryJournal
     *
     * @return Singelton instance of the RetryJournal
     */
    static RetryJournal& instance() {
      static RetryJournal instance;
      return instance;
    }

    /**
     * @brief Adds new entry to the journal
     *
     * @param pkt Packet to be added to the journal
     */
    void addEntry(const DataPacket& pkt);

    /**
     * @brief Deletes the entry from the journal
     *
     * @param pkt_id ID of the packet whoes entry must be deleted
     * @return True, if the deletion has been successful
     */
    bool deleteEntry(const uint16_t pkt_id);

    /**
     * @brief Re-Sends all of the needed packets
     */
    void executeRetries();
};


#endif // !RETRY_JOURNAL
