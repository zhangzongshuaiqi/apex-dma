#pragma once

#include <memory>

template <class T>
class Singleton {
public:
  template <typename... Args>
  constexpr static inline T* create(Args&&... args) {
    if (!_instance)
      _instance = std::make_unique<T>(std::forward<Args>(args)...);

    return _instance.get();
  }

  constexpr static inline void destroy() {
    if (_instance)
      _instance.reset();

    _instance = nullptr;
  }

  constexpr static inline T* get() { return _instance.get(); }

protected:
  Singleton() = default;
  ~Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton(const Singleton&&) = delete;

  Singleton& operator=(const Singleton&) = delete;
  Singleton& operator=(const Singleton&&) = delete;

private:
  static std::unique_ptr<T> _instance;
};

template <class T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

#define SINGLETON(name) \
  name : public         \
  Singleton<name>
