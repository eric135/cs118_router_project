#include "nat.hpp"
#include "core/utils.hpp"
#include "core/interface.hpp"
#include "simple-router.hpp"

#include <algorithm>
#include <iostream>

namespace simple_router {

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// IMPLEMENT THESE METHODS
void
NatTable::checkNatTable()
{
}

std::shared_ptr<NatEntry>
NatTable::lookup(uint16_t id)
{
  return nullptr;
}


void
NatTable::insertNatEntry(uint16_t id, uint32_t in_ip, uint32_t ex_ip)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// You should not need to touch the rest of this code.

NatTable::NatTable(SimpleRouter& router)
  : m_router(router)
  , m_shouldStop(false)
  , m_tickerThread(std::bind(&NatTable::ticker, this))
{
}

NatTable::~NatTable()
{
  m_shouldStop = true;
  m_tickerThread.join();
}


void
NatTable::clear()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_natTable.clear();
}

void
NatTable::ticker()
{
  while (!m_shouldStop) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
      std::lock_guard<std::mutex> lock(m_mutex);

      auto now = steady_clock::now();

      std::map<uint16_t, std::shared_ptr<NatEntry>>::iterator entryIt;
      for (entryIt = m_natTable.begin(); entryIt != m_natTable.end(); entryIt++ ) {
        if (entryIt->second->isValid && (now - entryIt->second->timeUsed > SR_ARPCACHE_TO)) {
          entryIt->second->isValid = false;
        }
      }

      checkNatTable();
    }
  }
}

std::ostream&
operator<<(std::ostream& os, const NatTable& table)
{
  std::lock_guard<std::mutex> lock(table.m_mutex);

  os << "\nID            Internal IP         External IP             AGE               VALID\n"
     << "-----------------------------------------------------------------------------------\n";

  auto now = steady_clock::now();

  for (auto const& entryIt : table.m_natTable) {
    os << entryIt.first << "            "
       << ipToString(entryIt.second->internal_ip) << "         "
       << ipToString(entryIt.second->external_ip) << "         "
       << std::chrono::duration_cast<seconds>((now - entryIt.second->timeUsed)).count() << " seconds         "
       << entryIt.second->isValid
       << "\n";
  }
  os << std::endl;
  return os;
}

} // namespace simple_router
