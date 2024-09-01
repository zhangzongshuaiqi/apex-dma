#pragma once

#include <pch.hpp>

namespace projects::visuals {
void routine();

/// @todo Reorganize this function, can be moved into a more general math class
std::optional<glm::ivec2> world_to_screen(const glm::vec3& world, const glm::mat4& view);

std::optional<std::pair<glm::ivec2, glm::ivec2>> bounding_box(const glm::mat4x3& matrix,
                                                              const glm::vec3& mins,
                                                              const glm::vec3& maxs,
                                                              const glm::mat4& view);
}  // namespace projects::visuals