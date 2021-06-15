#pragma once
#include "Engine/Core/Event.h"

namespace GuiLayer {
	void Init();
	void Destroy();

	void Begin();
	void End();

	bool handledEvent(Event& event);
}