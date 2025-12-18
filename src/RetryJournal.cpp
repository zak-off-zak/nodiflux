#include "RetryJournal.hpp"
#include "DataPacket.hpp"
#include <ctime>
#include <memory>
#include <mutex>


void RetryJournal::addEntry(const DataPacket& pkt){
  std::lock_guard<std::mutex> lock(this->mtx);

  std::unique_ptr<RetryJournalEntry> new_entry = std::unique_ptr<RetryJournalEntry>(
      new RetryJournalEntry{
        std::unique_ptr<DataPacket>(new DataPacket(pkt)),
        std::time(nullptr),
        0
      }
      );

  this->entries.emplace(pkt.getPacketId(), std::move(new_entry));
}


bool RetryJournal::deleteEntry(const uint16_t pkt_id){
  std::lock_guard<std::mutex> lock(this->mtx);
  return this->entries.erase(pkt_id) > 0;
}

