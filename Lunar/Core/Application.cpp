//
// Created by Minkyeu Kim on 7/21/23.
//

// https://github.com/StudioCherno/Walnut/blob/3b8e414fdecfc6c8b58816106fe8d912bd172e31/Walnut/src/Walnut/Application.cpp
#include "Application.h"
#include "Log.h" // 이건 cpp에만 설정.

extern bool g_ApplicationRunning;
static Lunar::Application* s_Instance = nullptr; // Single instance

static void glfw_error_callback(int error_code, const char* description) noexcept
{
	LOG_ERROR("GLFW Error {0}: {1}", error_code, description);
}

namespace Lunar {

	Application::Application(const Lunar::ApplicationSpecification& appSpec) noexcept
		: m_Specification(appSpec)
	{   // https://stackoverflow.com/questions/51705967/advantages-of-pass-by-value-and-stdmove-over-pass-by-reference
        if (s_Instance == nullptr) // 이미 app이 하나 켜져있으면 더이상 키지 말 것!
        {
            s_Instance = this;
            Init();
        }
        else
        {
            LOG_ERROR("Duplicate Application instance");
            assert(false);
        }
	}

	// Initialize GLFW window
	// https://github.com/TheCherno/OpenGL/blob/master/OpenGL-Core/src/Platform/Windows/WindowsWindow.cpp#L20
	// https://github.com/StudioCherno/Walnut/blob/20f940b9d23946d4836b8549ff3e2c0750c5d985/Walnut/src/Walnut/Application.cpp
	void Application::Init() noexcept
	{
		// Setup GLFW window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) // Init GLFW
		{
			LOG_ERROR("GLFW initialization failed...");
			return;
		}
		// Setup GLFW window properties
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL major version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL minor version
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile = No Backwards Compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

		m_Window.Handle = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Specification.Name, nullptr, nullptr);
		if (!m_Window.Handle)
		{
			glfwTerminate();
			assert(false && "GLFW window creation failed");
		}
		// set the current context
		glfwMakeContextCurrent(m_Window.Handle); // Set context for GLEW to use (그럼 multiple context 존재? 여러 화면?)

		// set ImGui
		// https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Control.
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // IF using Docking Branch
//		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

//		ImGuiStyle& style = ImGui::GetStyle();
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//		{
//			style.WindowRounding = 0.0f;
//			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//		}
		// set ImGui Style (Dark or Light)
		ImGui::StyleColorsDark();

		// setup platform/renderer backends
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui_ImplGlfw_InitForOpenGL(m_Window.Handle, true);

		// 윈도우에 따라 getWindowUserPointer가 반환해줄 값을 설정(나를 위함) --> 이후 callback에서 사용.
		glfwSetWindowUserPointer(m_Window.Handle, this);
		// [SWAP BUFFER SYNC] : https://www.glfw.org/docs/3.3/quick.html#quick_swap_buffers
		glfwSwapInterval(1);
		// Get Buffer size information (data for window rendering) : https://www.glfw.org/docs/3.3/window_guide.html#window_fbsize
		glfwGetFramebufferSize(m_Window.Handle, &m_Window.BufferWidth, &m_Window.BufferHeight);

		// TODO: remove later!
		glewExperimental = GL_TRUE; // Allow modern extension features
		if (glewInit() != GLEW_OK) // Check init state
		{
			glfwDestroyWindow(m_Window.Handle);
			glfwTerminate();
			assert(false && "glew initialization failed");
		}
		// TODO: remove later!
		glEnable(GL_DEPTH_TEST); // 약간 야매 방식. depth buffer 없이 실시간 검사로 일단 테스트 (임시 방편)
		glViewport(0, 0, m_Window.BufferWidth, m_Window.BufferHeight); // Setup Viewport size (OpenGL functionality)

		// set GLFW callbacks
		/*
		glfwSetWindowSizeCallback(m_Window.Handle,[](GLFWwindow* currentWindow, int width, int height) -> void
		{
			auto app = (Lunar::Application *)glfwGetWindowUserPointer(currentWindow);
			app->m_Window.BufferWidth = width;
			app->m_Window.BufferHeight = height;
//			LOG_TRACE("Window Resize: W={0} H={0}", width, height);
			for (auto &layer : app->m_LayerStack)
			{
				layer->OnWindowResize((float)width, (float)height);
			}
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((int)width, (int)height);
			glViewport(0, 0, width, height);
		});
		*/

        // set GLFW callbacks
		glfwSetWindowCloseCallback(m_Window.Handle,[](GLFWwindow* window) -> void
		{
			auto app = (Lunar::Application *)glfwGetWindowUserPointer(window);
			app->Shutdown();
		});

		glfwSetScrollCallback(m_Window.Handle, [](GLFWwindow* window, double xOffset, double yOffset) -> void
		{
			LOG_TRACE("Mouse Scroll: xOffset={0} yOffset={0}", xOffset, yOffset);
		});

        // set GLFW callbacks
        glfwSetMouseButtonCallback(m_Window.Handle, [](GLFWwindow* window, int button, int action, int mode) -> void
        {
		   // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
		   ImGuiIO& io = ImGui::GetIO();
		   io.AddMouseButtonEvent(button, action);

		   // (2) ONLY forward mouse data to your underlying app/game.
		   if (!io.WantCaptureMouse)
		   {
//			   my_game->HandleMouseData(...);
		   }
	   });

        // set GLFW callbacks
        glfwSetCursorPosCallback(m_Window.Handle, [](GLFWwindow* window, double xPos, double yPos) -> void
        {
			 // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
			 ImGuiIO& io = ImGui::GetIO();
			 io.AddMousePosEvent(xPos, yPos);
		 });

        // set GLFW callbacks
        glfwSetKeyCallback(m_Window.Handle,[](GLFWwindow* window, int key, int code, int action, int mode) -> void
        {
			if (Input::IsKeyPressed(Key::Escape))
			{
				auto app = (Lunar::Application *)glfwGetWindowUserPointer(window);
				app->Shutdown();
			}
		});

		LOG_INFO("OpenGL Info:");
		LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    }

    GLFWwindow* Application::GetWindowHandle() const noexcept
    {
        return m_Window.Handle;
    }

    // https://github.com/StudioCherno/Walnut/blob/20f940b9d23946d4836b8549ff3e2c0750c5d985/Walnut/src/Walnut/Application.cpp#L554
	// https://github.com/TheCherno/OpenGL/blob/master/OpenGL-Core/src/GLCore/Core/Application.cpp
    void Application::Run() noexcept
    {
        m_Running = true;
//		ImGuiIO& io = ImGui::GetIO();

		// NOTE: This is main render loop (m_Running을 먼저 끊어줘야 한다)
        while (!glfwWindowShouldClose(m_Window.Handle) && m_Running)
        {
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();

			// NOTE: Update every render layer
			for (auto& layer : m_LayerStack) {
				layer->OnUpdate(m_TimeStep);
			}

			// NOTE: Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// https://github.com/StudioCherno/Walnut/blob/20f940b9d23946d4836b8549ff3e2c0750c5d985/Walnut/src/Walnut/Application.cpp

			{
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
				if (m_MenubarCallback) {
					window_flags |= ImGuiWindowFlags_MenuBar;
				}
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
//				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowSize(ImVec2(500, 500));
//				LOG_INFO("{0} {1}", viewport->WorkSize.x, viewport->WorkSize.y);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
				// and handle the pass-thru hole, so we ask Begin() to not render a background.
				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
					window_flags |= ImGuiWindowFlags_NoBackground;
				}

				// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
				// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
				// all active windows docked into it will lose their parent and become undocked.
				// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
				// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace Demo", nullptr, window_flags);
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				// Submit the DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("DockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				if (m_MenubarCallback)
				{
					if (ImGui::BeginMenuBar())
					{
						m_MenubarCallback();
						ImGui::EndMenuBar();
					}
				}

				for (auto& layer : m_LayerStack) {
					layer->OnUIRender();
				}
				ImGui::End();
			}

			// NOTE: Rendering GUI
			// (Your code clears your framebuffer, renders your other stuff etc.)
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// Update and Render additional Platform Windows
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

            // NOTE: Swap GLFW Buffer
			glfwSwapBuffers(m_Window.Handle);

            // NOTE: Update time past (for animation)
            float time = Application::GetTime();
            m_FrameTime = time - m_LastFrameTime;
            m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
            m_LastFrameTime = time;
        }
    }

	Application::~Application() noexcept
	{
		// clear layer stack
		for (auto& layer : m_LayerStack) {
			layer->OnDetach();
		}
		m_LayerStack.clear();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window.Handle);
		glfwTerminate();

		s_Instance = nullptr;
	}

	// NOTE: 이렇게 close로 나누는 이유는 반복문 때문이다. Render Loop의 glfwPollEvent를 먼저 끊어줘야 한다.

	void Application::StopRenderLoop() noexcept
	{
		m_Running = false;
	}

    void Application::Shutdown() noexcept
	{
		LOG_INFO("Application::Shutdown");
		this->StopRenderLoop();
		g_ApplicationRunning = false;
    }

	void Application::Reboot() noexcept
	{
		LOG_INFO("Application::Reboot");
		this->StopRenderLoop();
		g_ApplicationRunning = true;
	}

    /* static function */
    float Application::GetTime() noexcept
    {
        return (float)glfwGetTime();
    }

    /* static function */
    Application& Application::Get() noexcept
    {
        return *s_Instance;
    }

	const WindowData& Application::GetWindowData() const
	{
		return m_Window;
	}

}