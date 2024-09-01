#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <nameof.hpp>

#include <memory>

template <class T>
class LoggerSingleton {
public:
  template <typename... Args>
  constexpr static inline T* create(Args&&... args) {
    if (!_instance) {
      logger = spdlog::stdout_color_mt(std::string(NAMEOF_TYPE(T)));

      logger->set_level(spdlog::level::debug);
      logger->set_pattern("[%r] [%^%n%$] %v");

      _instance = std::make_unique<T>(std::forward<Args>(args)...);
    }

    return _instance.get();
  }

  constexpr static inline void destroy() {
    if (_instance)
      _instance.reset();

    _instance = nullptr;
  }

  constexpr static inline T* get() { return _instance.get(); }

protected:
  LoggerSingleton() = default;
  ~LoggerSingleton() = default;

  LoggerSingleton(const LoggerSingleton&) = delete;
  LoggerSingleton(const LoggerSingleton&&) = delete;

  LoggerSingleton& operator=(const LoggerSingleton&) = delete;
  LoggerSingleton& operator=(const LoggerSingleton&&) = delete;

  static std::shared_ptr<spdlog::logger> logger;

private:
  static std::unique_ptr<T> _instance;
};

template <class T>
std::unique_ptr<T> LoggerSingleton<T>::_instance{};
template <class T>
std::shared_ptr<spdlog::logger> LoggerSingleton<T>::logger{};

#define LOGGER(name) \
  name : public      \
  LoggerSingleton<name>