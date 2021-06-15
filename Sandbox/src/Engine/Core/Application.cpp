#include "xpch.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Engine/Debug/MemoryTracker.h"
#include "Window.h"
#include "Event.h"
#include "Engine/Codes.h"
#include "Engine/Gui/GuiLayer.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Engine/Debug/Logger.h"
#include "Engine/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Scene.h"

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"





#if defined(DEBUG) || defined(RELEASE)
extern void engineGuiUpdate();
#endif

extern void onStartup();
extern void onShutdown();

static Window* s_window;
static bool s_isRunning = true;
static double s_lastTime = 0.0; //last time recorded (a time is recorded each frame)
static bool s_isFocused = true;

static bool s_displayDebugWindow = false;

//compute the FPS every second rather than every frame
static int s_frameCount = 0;



std::unordered_map<const char*, Scene*>* s_Scenes = nullptr;
static Scene* s_currentScene;



extern ApplicationProperties createApplication();




#if defined(DEBUG) || defined(RELEASE)
void engineGuiUpdate() { //enocurperae into the loop
	if (!s_displayDebugWindow)
		return;


	ImGui::Begin("xEngine Debug");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	

	ImGui::Text("Total Mem Allocated: %d bytes", get_total_memory_allocated());
	ImGui::Text("Total Mem Deallocated: %d bytes", get_total_memory_freed());
	ImGui::Text("Current Mem Allocated: %d bytes", get_current_memory_allocated());
	
	ImGui::Text("Draw calls: %d",Renderer::getDrawCallsPerFrame());
	
	
	ImGui::Text("Active Scene: '%s'",s_currentScene->getName());


	if (ImGui::Button("Force close")) {
		exit(0);
	}
	ImGui::End();
	
}
#endif



static bool resize(Event& event) {
	WindowResizeEvent& resize_event = *(WindowResizeEvent*)(&event);
	s_window->setHeight(resize_event.get_height());
	s_window->setWidth(resize_event.get_width());
	return false;
}

static bool close(Event& closeEvent) {
	if (!closeEvent.m_handled)
	{
		closeEvent.m_handled = true;
		s_isRunning = false;
	}
	return true;
}

void end() {
	
	GuiLayer::Destroy();
	delete s_window;
	s_window = nullptr;
	
	Renderer::Shutdown();


	glfwTerminate();
	onShutdown();
	delete s_Scenes;
}

static bool set_focus(Event& event) {
	s_isFocused = true;
	s_lastTime = glfwGetTime();
	return true;
}

static bool remove_focus(Event& event) {
	s_isFocused = false;
	return true;
}

static void onEvent(Event& event) {

	if (Event::isEvent<WindowSetFocusEvent>(event) && !event.m_handled)
		set_focus(event);

	if (!s_isFocused) 
		event.m_handled = true;
	
	if (event.m_handled)
		return;

	switch (event.get_type())
	{
	case WindowCloseEvent::getEventType() :
		close(event);
		break;
	case WindowResizeEvent::getEventType() :
		resize(event);
		break;
	case WindowRemoveFocusEvent::getEventType(): 
		remove_focus(event);
		break;

	};

	if (event.m_handled) //dont send it to the layers if is already handled
		return;

	//check if the event is engine-specific ctrl-function key
	switch (event.get_type()) {
	case Event::EventType::key_press:
		if (Event::EventCast<KeyPressEvent>(event).get_keyCode() == codes::KeyCode::f5) //EVENT(KeyPressedEvent, event).get_keyCode() == codes::KeyCode::f5
			if (Input::keyPressed(codes::KeyCode::left_control))
				s_displayDebugWindow = !s_displayDebugWindow;
		break;
	}

	s_currentScene->pushEvent(event);
}

void GLAPIENTRY OpenglDebugCallFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	const char* s_source = "";
	switch (((int)source)) {
	case GL_DEBUG_SOURCE_API :
		s_source = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM :
		s_source = "Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER :
		s_source = "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY :
		s_source = "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION :
		s_source = "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER :
		s_source = "Other";
		break;
	}

	const char* s_type = "";
	switch (((int)(type))) {
	case GL_DEBUG_TYPE_ERROR:
		s_type = "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		s_type = "Deprecated Behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		s_type = "Undefined Behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		s_type = "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		s_type = "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		s_type = "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		s_type = "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		s_type = "Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		s_type = "Other";
		break;
	}

	const char* s_severity = "";
	switch (((int)(severity))) {
	case GL_DEBUG_SEVERITY_LOW:
		s_severity = "Low";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		s_severity = "Medium";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		s_severity = "High";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		s_severity = "Notification";
		break;

	}


	Log_EngineError("OpenGL Error\nSource: {0}\nType: {1}\nID: {2}\nSeverity: {3}\n{4}\n",s_source, s_type, id, s_severity, message);
}

static void init(std::string name, unsigned int width, unsigned int height) {
	s_window = new Window({ name, width, height });
	s_window->set_event_callback(onEvent);
	s_Scenes = new std::unordered_map<const char*, Scene*>;


	glEnable(GL_DEBUG_OUTPUT);

	glDebugMessageCallback(OpenglDebugCallFunc, 0);
	
	ShaderType::Init();
	GuiLayer::Init();
	Renderer::Init();

}

Window* application::getWindow() { return s_window; }

void application::SetActiveScene(const char* sceneName)
{
	auto search = s_Scenes->find(sceneName);
	if (search == s_Scenes->end())
	{
		Log_EngineWarn("Scenes not created or already deleted - {0}", sceneName);
		return;
	}

	s_currentScene = (*s_Scenes)[sceneName];

}

Scene& application::getActiveScene()
{
	return *s_currentScene;
}

Scene& application::getScene(const char* sceneName)
{
	auto search = s_Scenes->find(sceneName);
	if (search == s_Scenes->end())
	{
		Log_EngineError("Scenes not created or already deleted - {0}", sceneName);
		Log_EngineWarn("Scenes named ' {0} ' created to avoid program crash", sceneName);
		Scene* scene = new Scene(sceneName);
		(*s_Scenes)[sceneName] = scene;
		return *scene;
	}
	return *(*s_Scenes)[sceneName];
}

void application::createScene(const char* sceneName)
{
	//scenes cannot share the same name
	auto search = s_Scenes->find(sceneName);
	if (search != s_Scenes->end())
	{
		Log_EngineWarn("Scenes may not share the same name - {0}",sceneName);
		return;
	}
	Scene* new_scene = new Scene(sceneName);
	(*s_Scenes)[sceneName] = new_scene;
}

void application::deleteScene(const char* sceneName)
{
	auto search = s_Scenes->find(sceneName);
	if (search == s_Scenes->end())
	{
		Log_EngineWarn("Scenes not created or already deleted - {0}", sceneName);
		return;
	}
	Scene* scene = (*s_Scenes)[sceneName];
	delete scene;
	s_Scenes->erase(sceneName);
}

static void start() {
	onStartup();
	static double s_CurrentTime;
	while (s_isRunning) {
		s_window->onUpdate();

		if (s_isFocused) {
			s_CurrentTime = glfwGetTime();
			s_currentScene->onUpdate(DeltaTime(s_CurrentTime - s_lastTime - application::getWindow()->getLastPollTime()));
			s_lastTime = s_CurrentTime;
		}
	}

}





#ifdef DEBUG
#define ENTRY_POINT main()
#else
#if defined(_WIN32) || defined(_WIN64)
#define ENTRY_POINT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
#define ENTRY_POINT main()
#endif
#endif

int ENTRY_POINT
{
	LOG_INIT("Engine","Client");
	START_MEM_TRACK();

	ApplicationProperties props = createApplication();

	init(props.title,props.width,props.height);
	start();
	end();

	END_MEM_TRACK();
	LOG_DESTROY();

	std::cin.get();
}