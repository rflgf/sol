#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

namespace sol::ecl::diu
{

bool drag_float(const std::string label, float *v, const char *format = "%.2f",
                float button_width = 80.0f);
void separator();
void pretty_vector_3(std::string label, glm::vec3 &values,
                     float reset_value = 0.0f, float column_width = 86.0f);
void easy_theming(glm::vec3 color_for_text, glm::vec3 color_for_head,
                  glm::vec3 color_for_area, glm::vec3 color_for_body,
                  glm::vec3 color_for_pops);

} // namespace sol::ecl::diu