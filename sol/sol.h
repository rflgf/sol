#pragma once

// Intended to be the only header included by the client application, which
// needs to define GetApplication (see EntryPoint.h for more details).

// NOTE: define this exactly once before including this file.
#ifdef SOL_ENTRY_POINT_SOURCE_FILE
#include "EntryPoint.h"
#endif

#include "Application.h"
#include "CameraController.h"
#include "Core.h"
#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Input.h"
#include "KeyCodes.h"
#include "Layer.h"
#include "Log.h"
#include "Renderer/Buffers.h"
#include "Renderer/Camera.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/Subtexture2D.h"
#include "Renderer/Texture.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneSerializer.h"
#include "Scene/ScriptableEntity.h"
#include "Utils.h"
#include "Window.h"
