#pragma once

#include <pch.hpp>

#include <imgui.h>

namespace backend {
class LOGGER(Config) {
public:
  using ConfigStorage = std::unordered_map<std::string,
                                           std::variant<bool,
                                                        int32_t,
                                                        float,
                                                        std::string,
                                                        ImColor,
                                                        std::unordered_map<std::string, bool>>>;
  using ConfigExtension = void(ConfigStorage&);

  Config(std::function<ConfigExtension> extension = [](ConfigStorage&) {}) : _extension(extension) {
    if (!std::filesystem::exists("C:\\wednesday.wtf\\configs\\")) {
      logger->debug("Creating config directory");

      std::filesystem::create_directories("C:\\wednesday.wtf\\configs\\");
    }

    _config["menu_dark_mode"] = false;

    _extension(_config);
  };
  ~Config() = default;

  void load(const std::string& path);
  void save(const std::string& path);

  template <typename T>
  [[nodiscard]] static inline T& value(const std::string& key) {
    auto& var = get()->_config[key];

    return *std::get_if<T>(&var);
  }

private:
  ConfigStorage _config{};
  std::function<ConfigExtension> _extension{};
};
}  // namespace backend

#define CONFIG_VALUE(type, key) backend::Config::value<type>(key)
#define STATIC_CONFIG_VALUE(var, type, key) static auto& var = CONFIG_VALUE(type, key)