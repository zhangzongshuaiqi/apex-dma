#include <config.hpp>

void backend::Config::load(const std::string& path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    file.close();

    logger->error("Failed to open config file: {}", path);

    return;
  }

  if (file.peek() == std::ifstream::traits_type::eof()) {
    file.close();

    logger->error("Config file is empty: {}", path);

    return;
  }

  nlohmann::json json;

  file >> json;

  auto& config = _config;

  for (const auto& [key, value] : _config) {
    if (!json.contains(key)) {
      logger->warn("Key not found in config: {}", key);

      continue;
    }

    std::visit(
        [&config, &key, &json](const auto& value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, bool>)
            config[key] = json[key].get<bool>();

          if constexpr (std::is_same_v<T, int32_t>)
            config[key] = json[key].get<int32_t>();

          if constexpr (std::is_same_v<T, float>)
            config[key] = json[key].get<float>();

          if constexpr (std::is_same_v<T, std::string>)
            config[key] = json[key].get<std::string>();

          if constexpr (std::is_same_v<T, ImColor>) {
            config[key] = ImColor(json[key]["r"].get<float>(), json[key]["g"].get<float>(),
                                  json[key]["b"].get<float>(), json[key]["a"].get<float>());
          }

          if constexpr (std::is_same_v<T, std::unordered_map<std::string, bool>>) {
            std::unordered_map<std::string, bool>& map =
                std::get<std::unordered_map<std::string, bool>>(config[key]);

            for (const auto& [k, v] : map) {
              if (!json[key].contains(k))
                continue;

              map[k] = json[key][k].get<bool>();
            }
          }
        },
        value);
  }

  file.close();
}

void backend::Config::save(const std::string& path) {
  nlohmann::json json;

  for (const auto& [key, value] : _config) {
    std::visit(
        [&json, &key](const auto& value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, bool>)
            json[key] = value;

          if constexpr (std::is_same_v<T, int32_t>)
            json[key] = value;

          if constexpr (std::is_same_v<T, float>)
            json[key] = value;

          if constexpr (std::is_same_v<T, std::string>)
            json[key] = value;

          if constexpr (std::is_same_v<T, ImColor>) {
            json[key] = {{"r", value.Value.x},
                         {"g", value.Value.y},
                         {"b", value.Value.z},
                         {"a", value.Value.w}};
          }

          if constexpr (std::is_same_v<T, std::unordered_map<std::string, bool>>) {
            for (const auto& [k, v] : value)
              json[key][k] = v;
          }
        },
        value);
  }

  std::ofstream file(path);

  if (!file.is_open()) {
    file.close();

    logger->error("Failed to open config file: {}", path);

    return;
  }

  file << json.dump(2);

  file.close();
}