#include "Panels.h"

#include "Core.h"
#include "DockspaceLayer.h"
#include "ImGuiUtils.h"
#include "Scene/Components.h"
#include "Scene/SceneCamera.h"

#include <SDL_stdinc.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <stdint.h>

namespace sol::ecl::pnl
{

#define IDIFY(x) std::bit_cast<void *>(uintptr_t {static_cast<uint32_t>(x)})

// SceneHierarchy --------------------------------------------------------------
void SceneHierarchy::on_imgui_update()
{
	SOL_CORE_ASSERT(context, "scene hierarchy context not set.");

	{
		ImGui::Begin("scene hierarchy");

		context->registry.each(
		    [this](entt::entity handle)
		    {
			    Entity entity(*context, handle);
			    draw_entity_node(entity);
		    });

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
			selection_context = {};

		// right-click on blank space.
		if (ImGui::BeginPopupContextWindow(nullptr,
		                                   ImGuiPopupFlags_MouseButtonRight |
		                                       ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("add entity"))
				context->create("empty entity");
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	{
		ImGui::Begin("properties");
		if (selection_context)
			draw_entity_properties(selection_context);

		ImGui::End();
	}
}

void SceneHierarchy::draw_entity_node(Entity entity)
{
	ImGuiTreeNodeFlags flags =
	    (selection_context == entity ? ImGuiTreeNodeFlags_Selected
	                                 : ImGuiTreeNodeFlags_None) |
	    ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	std::string tag = entity.get<cmp::Tag>().tag;
	bool opened  = ImGui::TreeNodeEx(IDIFY(entity), flags, "%s", tag.c_str());
	bool deleted = false;

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("delete entity"))
			deleted = true;
		ImGui::EndPopup();
	}

	if (ImGui::IsItemClicked())
		selection_context = entity;

	if (opened)
	{
		// TODO(rafael): unfold the hierarchy tree.
		ImGui::TreePop();
	}

	if (deleted)
	{
		context->destroy(entity);
		if (selection_context == entity)
			selection_context = {};
	}
}

void SceneHierarchy::draw_entity_properties(Entity entity)
{
	if (entity.has<cmp::Tag>())
	{
		std::string &tag                = entity.get<cmp::Tag>().tag;
		char buffer[cmp::Tag::MAX_SIZE] = {0};
		strcpy_s(buffer, cmp::Tag::MAX_SIZE, tag.c_str());
		ImGui::Text("tag:");
		ImGui::InputText("##tag", buffer, cmp::Tag::MAX_SIZE);
		tag = std::string(buffer);
	}

	{
		const char *id = "add component";
		if (ImGui::Button("+"))
			ImGui::OpenPopup(id);

		if (ImGui::BeginPopup(id))
		{
			if (!entity.has<cmp::SpriteRenderer>())
				if (ImGui::MenuItem("sprite renderer"))
				{
					selection_context.add<cmp::SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}

			if (!entity.has<cmp::Camera>())
				if (ImGui::MenuItem("camera"))
				{
					selection_context.add<cmp::Camera>();
					ImGui::CloseCurrentPopup();
				}

			// native script instances.
			if (ImGui::MenuItem("native script"))
			{
			}

			ImGui::EndPopup();
		}
	}

	if (entity.has<cmp::Transform>())
	{
		diu::separator();

		if (ImGui::TreeNodeEx(IDIFY(typeid(cmp::Transform).hash_code()),
		                      ImGuiTreeNodeFlags_DefaultOpen |
		                          ImGuiTreeNodeFlags_SpanAvailWidth,
		                      "transform"))
		{
			cmp::Transform &transform = entity.get<cmp::Transform>();
			diu::pretty_vector_3("translation", transform.translation);
			glm::vec3 rotation = glm::degrees(transform.rotation);
			diu::pretty_vector_3("rotation", rotation);
			transform.rotation = glm::radians(rotation);
			diu::pretty_vector_3("scale", transform.scale, 1.0f);

			if (ImGui::Button("reset"))
				entity.replace<cmp::Transform>();

			ImGui::TreePop();
		}
	}

	if (entity.has<cmp::Camera>())
	{
		diu::separator();

		if (ImGui::TreeNodeEx(IDIFY(typeid(cmp::Camera).hash_code()),
		                      ImGuiTreeNodeFlags_DefaultOpen, "camera"))
		{
			cmp::Camera &component                = entity.get<cmp::Camera>();
			SceneCamera &camera                   = component.camera;
			const char *projection_type_strings[] = {"orthographic",
			                                         "perspective"};
			const char *current_projection_type_string =
			    projection_type_strings[static_cast<int>(
			        camera.get_projection_type())];
			ImGui::Text("projection type");
			if (ImGui::BeginCombo("##projection type",
			                      current_projection_type_string))
			{
				for (int i = 0; i < 2; ++i)
				{
					bool selected =
					    static_cast<int>(camera.get_projection_type()) == i;
					if (selected)
						ImGui::SetItemDefaultFocus();
					if (ImGui::Selectable(projection_type_strings[i], selected))
						camera.set_projection_type(
						    static_cast<SceneCamera::Type>(i));
				}
				ImGui::EndCombo();
			}

			bool primary_camera = component.primary;
			if (ImGui::Checkbox("primary camera", &primary_camera))
				component.primary = !component.primary;

			if (camera.get_projection_type() == SceneCamera::Type::ORTHOGRAPHIC)
			{
				float orthographic_size = camera.get_orthographic_size();
				float orthographic_near = camera.get_orthographic_near();
				float orthographic_far  = camera.get_orthographic_far();

				if (diu::drag_float("size", &orthographic_size))
					camera.set_orthographic_size(orthographic_size);
				if (diu::drag_float("near clip", &orthographic_near))
					camera.set_orthographic_near(orthographic_near);
				if (diu::drag_float("far clip", &orthographic_far))
					camera.set_orthographic_far(orthographic_far);
			}
			else
			{
				float vertical_fov = glm::degrees(
				    camera.get_perspective_vertical_field_of_view());
				float perspective_near = camera.get_perspective_near();
				float perspective_far  = camera.get_perspective_far();

				if (diu::drag_float("vertical field of view", &vertical_fov))
					camera.set_perspective_vertical_field_of_view(
					    glm::radians(vertical_fov));
				if (diu::drag_float("near clip", &perspective_near))
					camera.set_perspective_near(perspective_near);
				if (diu::drag_float("far clip", &perspective_far))
					camera.set_perspective_far(perspective_far);
			}

			bool fixed_aspect_ratio = component.fixed_aspect_ratio;
			if (ImGui::Checkbox("fixed aspect ratio", &fixed_aspect_ratio))
				component.fixed_aspect_ratio = !component.fixed_aspect_ratio;

			ImGui::TreePop();
		}
	}

	if (entity.has<cmp::SpriteRenderer>())
	{
		diu::separator();

		cmp::SpriteRenderer &component = entity.get<cmp::SpriteRenderer>();
		ImGui::Text("tint color");
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {4, 2});
		ImGui::ColorEdit4("##tint color", glm::value_ptr(component.color));
		ImGui::PopFont();
		ImGui::PopStyleVar();
	}

	// TODO(rafael): transform component.
}
// end of SceneHierarchy
// -------------------------------------------------------

#undef IDIFY

} // namespace sol::ecl::pnl