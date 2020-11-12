#ifndef SIMPLE_ROUTER_NAT_HPP
#define SIMPLE_ROUTER_NAT_HPP

#include "core/protocol.hpp"

#include <map>
#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>

namespace simple_router {

class SimpleRouter;

using steady_clock = std::chrono::steady_clock;
using time_point = std::chrono::steady_clock::time_point;
using seconds = std::chrono::seconds;

const seconds SR_NAT_TO = seconds(30);

struct NatEntry {
  uint32_t internal_ip = 0; //NAT internal IP address 
  uint32_t external_ip = 0; //NAT external IP address
  time_point timeUsed; //The last time this entry used
  bool isValid = false; 
};

class NatTable {
public:
  NatTable(SimpleRouter& router);

  ~NatTable();

  /**
   * Checks table entries and removes expired entries.
   */
  void
  checkNatTable();

  /**
   * Looks up and returns a matching entry in the NAT table by using the key (id)
   */
  std::shared_ptr<NatEntry>
  lookup(uint16_t id);

  /**
   * Inserts a new NAT entry in the NAT table by creating a new NatEntry
   */
  void
  insertNatEntry(uint16_t id, uint32_t in_ip, uint32_t ex_ip);

  void
  clear();

private:
  void
  ticker();

private:
  SimpleRouter& m_router;

  std::map<uint16_t, std::shared_ptr<NatEntry>> m_natTable;

  volatile bool m_shouldStop;
  std::thread m_tickerThread;
  mutable std::mutex m_mutex;

  friend std::ostream&
  operator<<(std::ostream& os, const NatTable& table);
};

std::ostream&
operator<<(std::ostream& os, const NatTable& table);

} // namespace simple_router

#endif // SIMPLE_ROUTER_NAT_HPP
