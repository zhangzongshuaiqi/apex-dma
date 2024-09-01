#pragma once

#include <pch.hpp>

namespace backend {
class SINGLETON(Stopper) {
public:
  Stopper() = default;
  ~Stopper() = default;

  inline bool stop() const { return _stop_source.request_stop(); }
  [[nodiscard]] inline bool stop_requested() const { return _stop_source.stop_requested(); }

private:
  std::stop_source _stop_source{};
};
}  // namespace backend