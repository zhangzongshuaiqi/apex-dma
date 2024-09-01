#pragma once

#include <pch.hpp>

namespace projects {

class EntityEntry {
public:
  int64_t* entity{};  // 0x0000
private:
  char _pad_0008[24]{};  // 0x0008
public:
};  // Size: 0x0020
static_assert(sizeof(EntityEntry) == 0x20);

class Player {
public:
private:
  char _pad_0000[56]{};  // 0x0000
public:
  int64_t name_index{};  // 0x0038
private:
  char _pad_0040[304]{};  // 0x0040
public:
  glm::vec3 velocity{};  // 0x0170
  glm::vec3 origin{};    // 0x017C
private:
  char _pad_0188[24]{};  // 0x0188
public:
  int32_t shield{};      // 0x01A0
  int32_t shield_max{};  // 0x01A4
private:
  char _pad_01A8[368]{};  // 0x01A8
public:
  int32_t health{};  // 0x0318
private:
  char _pad_031C[12]{};  // 0x031C
public:
  int32_t team{};  // 0x0328
private:
  char _pad_032C[140]{};  // 0x032C
public:
  glm::vec3 mins{};  // 0x03B8
  glm::vec3 maxs{};  // 0x03C4
private:
  char _pad_03D0[144]{};  // 0x03D0
public:
  int32_t health_max{};  // 0x0460
private:
  char _pad_0464[540]{};  // 0x0464
public:
  int32_t life_state{};  // 0x0680
private:
  char _pad_0684[300]{};  // 0x0684
public:
  glm::mat3x4 rgfl{};  // 0x07B0
private:
  char _pad_07E0[4436]{};  // 0x07E0
public:
  int32_t primary_weapon_handle{};  // 0x1934
private:
  char _pad_1938[1432]{};  // 0x1938
public:
  glm::vec3 camera_origin{};  // 0x1ED0
  glm::vec3 camera_angles{};  // 0x1EDC
private:
  char _pad_1EE8[2708]{};  // 0x1EE8
public:
  int32_t buttons{};  // 0x297C
};  // Size: 0x2980
static_assert(sizeof(Player) == 0x2980);

class PlayerCache {
public:
  glm::vec3 velocity{};             // 0x0000
  glm::vec3 origin{};               // 0x000C
  int32_t shield{};                 // 0x0018
  int32_t shield_max{};             // 0x001C
  int32_t health{};                 // 0x0020
  int32_t team{};                   // 0x0024
  glm::vec3 mins{};                 // 0x0028
  glm::vec3 maxs{};                 // 0x0034
  int32_t health_max{};             // 0x0040
  int32_t life_state{};             // 0x0044
  glm::mat4x3 rgfl{};               // 0x0048 NOTE: Should be a 4x3 matrix!
  int32_t primary_weapon_handle{};  // 0x0078
  glm::vec3 camera_origin{};        // 0x007C
  glm::vec3 camera_angles{};        // 0x0088
  int32_t buttons{};                // 0x0094
  std::string name{};               // 0x0098 NOTE: Should be a std::string!
};  // Size: 0x00B8
static_assert(sizeof(PlayerCache) == 0xB8);
}  // namespace projects