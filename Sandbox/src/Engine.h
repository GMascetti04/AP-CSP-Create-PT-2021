#pragma once
//Include this header in all places in which Engine is to be used

#include "Engine/Core/Application.h"
#include "Engine/Core/Event.h"
#include "Engine/Core/Scene.h"
#include "Engine/Core/Window.h"

#include "Engine/Debug/Logger.h"
#include "Engine/Debug/MemoryTracker.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Registry.h"

#include "Engine/Gui/GuiLayer.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/CameraController.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/Utilities/Vec.h"


#include "Engine/Codes.h"
#include "Engine/DeltaTime.h"
#include "Engine/Input.h"

#include "imgui.h"


#define ENGINE_HEADER