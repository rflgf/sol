#pragma once

// Intended to be the only header included by the client application, which
// needs to define GetApplication (see EntryPoint.h for more details).

// NOTE: define this exactly once before including this file.
#ifdef SOL_ENTRY_POINT_SOURCE_FILE
#	include "EntryPoint.h"
#endif

#include "Application.h"
#include "CameraController.h"
#include "Core.h"
#include "Input.h"
#include "KeyCodes.h"
#include "Layer.h"
#include "Log.h"
#include "Renderer/Buffers.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Utils.h"
#include "Window.h"
