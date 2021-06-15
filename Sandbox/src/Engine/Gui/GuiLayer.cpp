#include "xpch.h"
#include "GLFW/glfw3.h"
#include "Engine/Gui/GuiLayer.h"
#include "Engine/Core/Application.h"

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


/*
* Note: Code for this function directly models example provided in github repository for ImGui
* link: https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
*/
void GuiLayer::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(application::getWindow()->getNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

/*
* Note: Code for this function directly models example provided in github repository for ImGui
* link: https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
*/
void GuiLayer::Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/*
* Note: Code for this function directly models example provided in github repository for ImGui
* link: https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
*/
void GuiLayer::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

/*
* Note: Code for this function directly models example provided in github repository for ImGui
* link: https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
*/
void GuiLayer::End() {
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(application::getWindow()->getNativeWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static bool s_filter_events = true;

//returns true if the event was used by the gui layer
bool GuiLayer::handledEvent(Event& event) {
	if (s_filter_events)
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.WantCaptureMouse) {
			if (event.get_categories() & (Event::EventCategory::mouse | Event::EventCategory::mouse_button)) {
				event.m_handled = true;
				return true;
			}
		}
		if (io.WantCaptureKeyboard) {
			if (event.get_categories() & (Event::EventCategory::keyboard)) {
				event.m_handled = true;
				return true;
			}
		}
	}
	return false;
}