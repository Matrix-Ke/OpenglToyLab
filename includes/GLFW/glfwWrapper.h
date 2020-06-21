#pragma once
#include <string>


#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "util/eventManager.h"


namespace OpenGL
{
	class Glfw
	{
	public:
		static Glfw*  GetInstance();

		void Init(size_t width = 800, size_t height = 600, const std::string & title = "title");

		void Terminate();

		void RenderLoop(Oper::Operation* op = nullptr);

		void RenderLoop(Oper::Ptr<Oper::Operation> op);

		GLFWwindow*   GetWindow();
		void	CloseWindow();

		void	LockCursor();

		int GetKey(int key);
	protected:

	private:

		Glfw();
		Glfw(const Glfw&);
		Glfw&   operator=(const Glfw&) = default;

		static  Glfw* m_Instance;

		static  void  CB_FrameBuffSize(GLFWwindow* window, int width, int height);
		
		void    GenWindow(size_t width = 800, size_t height = 600, const std::string & title = "title");
		void    LoadGL();

		GLFWwindow* window;
	};
}