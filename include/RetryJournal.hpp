#ifndef RETRY_JOURNAL
#define RETRY_JOURNAL

#include "DataPacket.hpp"
#include <cstdint>
#include <ctime>
#include <memory>
#include <mutex>
#include <unordered_map>

struct RetryJournalEntry{
  std::unique_ptr<DataPacket> pkt;
  time_t timestamp;
  uint8_t retries;
};

class RetryJournal {
  private:
    std::unordered_map<uint16_t, std::unique_ptr<RetryJournalEntry>> entries;
    mutable std::mutex mtx;
  public:
    RetryJournal () = default;
    RetryJournal(const RetryJournal&) = delete;
    RetryJournal& operator=(const RetryJournal&) = delete;

    static RetryJournal& instance() {
      static RetryJournal instance;
      return instance;
    }

    void addEntry(const DataPacket& pkt);
    bool deleteEntry(const uint16_t pkt_id);
    void executeRetries();
};


#endif // !RETRY_JOURNAL
