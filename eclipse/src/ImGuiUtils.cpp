#include "ImGuiUtils.h"

namespace sol::ecl::diu
{

bool drag_float(const std::string label, float *v, const char *format,
                float button_width)
{
	ImGui::PushID(label.c_str());
	ImGuiIO &io = ImGui::GetIO();

	ImGui::Columns(2, NULL, false);
	ImGui::SetColumnWidth(0, ImGui::CalcTextSize(label.c_str()).x);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();
	ImGui::SetColumnWidth(1, button_width);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 2});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {14, 2});
	ImGui::PushFont(io.Fonts->Fonts[1]);
	bool changed = ImGui::DragFloat(std::string("##" + label).c_str(), v, 1.0f,
	                                0.0f, 0.0f, format);
	ImGui::PopStyleVar(2);
	ImGui::PopFont();
	ImGui::Columns(1);
	ImGui::PopID();
	return changed;
}

void separator()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
	                    {ImGui::GetStyle().ItemSpacing.x, 4.0f});
	ImGui::Dummy({0, 0});
	ImGui::Separator();
	ImGui::PopStyleVar();
}

void pretty_vector_3(std::string label, glm::vec3 &values, float reset_value,
                     float column_width)
{
	ImGuiIO &io = ImGui::GetIO();

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, NULL, false);
	ImGui::SetColumnWidth(0, column_width);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 2});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {14, 2});
	ImGui::PushFont(io.Fonts->Fonts[1]);
	ImVec2 label_button_size = {
	    io.Fonts->Fonts[1]->FontSize + GImGui->Style.FramePadding.y * 2,
	    io.Fonts->Fonts[1]->FontSize + GImGui->Style.FramePadding.y * 2};

	// x value.
	{
		// shades of red.
		ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});

		if (ImGui::Button("##x_button", label_button_size))
			values.x = reset_value;

		ImGui::SameLine();

		ImVec2 current_pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(
		    {current_pos.x - label_button_size.x, current_pos.y});
		ImGui::Text("x");
		ImGui::SetCursorPos(current_pos);

		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
	}

	ImGui::SameLine();

	// y value.
	{
		// shades of green.
		ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.2f, 1.0f});

		if (ImGui::Button("##y_button", label_button_size))
			values.y = reset_value;

		ImGui::SameLine();

		ImVec2 current_pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(
		    {current_pos.x - label_button_size.x, current_pos.y});
		ImGui::Text("y");
		ImGui::SetCursorPos(current_pos);

		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::DragFloat("##y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
	}

	ImGui::SameLine();

	// z value.
	{
		// shades of blue.
		ImGui::PushStyleColor(ImGuiCol_Button, {0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
		                      {0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.1f, 0.25f, 0.8f, 1.0f});

		if (ImGui::Button("##z_button", label_button_size))
			values.z = reset_value;

		ImGui::SameLine();

		ImVec2 current_pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(
		    {current_pos.x - label_button_size.x, current_pos.y});
		ImGui::Text("z");
		ImGui::SetCursorPos(current_pos);

		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::DragFloat("##z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
	}

	ImGui::PopStyleVar(2);
	ImGui::PopFont();

	ImGui::Columns(1);

	ImGui::PopID();
}

// From https://github.com/ocornut/imgui/issues/707#issuecomment-254424199
// (adapted):

// From https://github.com/procedural/gpulib/blob/master/gpulib_imgui.h

void easy_theming(glm::vec3 color_for_text, glm::vec3 color_for_head,
                  glm::vec3 color_for_area, glm::vec3 color_for_body,
                  glm::vec3 color_for_pops)
{
	ImGuiStyle &style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] =
	    ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] =
	    ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] =
	    ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
	style.Colors[ImGuiCol_Border] =
	    ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] =
	    ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Button] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Header] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] =
	    ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] =
	    ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] =
	    ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] =
	    ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDimBg] =
	    ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);
}

} // namespace sol::ecl::diu