#include "RetryJournal.hpp"
#include "DataPacket.hpp"
#include "Protocol.hpp"
#include "config.hpp"
#include <ctime>
#include <memory>
#include <mutex>
#include <vector>


void RetryJournal::addEntry(const DataPacket& pkt){
  std::lock_guard<std::mutex> lock(this->mtx);

  std::unique_ptr<RetryJournalEntry> new_entry = std::unique_ptr<RetryJournalEntry>(
      new RetryJournalEntry{
        std::unique_ptr<DataPacket>(new DataPacket(pkt)),
        std::time(nullptr),
        0
      }
      );

  if(this->entries.size() < RETRY_JOURNAL_SIZE){
    this->entries.emplace(pkt.getPacketId(), std::move(new_entry));
  }
}


bool RetryJournal::deleteEntry(const uint16_t pkt_id){
  std::lock_guard<std::mutex> lock(this->mtx);
  return this->entries.erase(pkt_id) > 0;
}

void RetryJournal::executeRetries(){
  std::vector<uint16_t> to_delete;
  std::vector<DataPacket> to_send;

  {
    std::lock_guard<std::mutex> lock(mtx);
    time_t now = std::time(nullptr);
    for(const auto& [pkt_id, entry] : this->entries){
      if(std::difftime(now, entry->timestamp) > RETRY_TIME_THRESHOLD){
        if(entry->retries < RETRY_CNT_THRESHOLD){
          entry->timestamp = now;
          entry->retries++;
          to_send.push_back(*entry->pkt);
        } else {
          to_delete.push_back(pkt_id);
        }
      }
    }

    for(auto pkt_id: to_delete){
      this->entries.erase(pkt_id);
    }
  }


  for(auto& pkt: to_send){
    sendPacket(pkt.getDest(), pkt);
  }

}

