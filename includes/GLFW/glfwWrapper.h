#pragma once
#include <string>


#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "util/eventManager.h"
#include "util/opQueue.h"


namespace OpenGL
{
	class Glfw
	{
	public:
		static Glfw*  GetInstance();

		void Init(size_t width = 800, size_t height = 600, const std::string & title = "title");

		void Terminate();

		void setRenderEnv(Oper::Ptr<Oper::Operation> op);

		void SetForwardProcess(Oper::Ptr<Oper::Operation> op);

		void SetPostProcess(Oper::Ptr<Oper::Operation> op);

		void SetFramerEndProcess(Oper::Ptr<Oper::Operation> op);

		void StartRenderLoop();

		GLFWwindow*   GetWindow();
		void	CloseWindow();

		void	LockCursor();

		int GetKey(int key);
	protected:
		void initProcess();

	private:

		Glfw();
		Glfw(const Glfw&);
		Glfw&   operator=(const Glfw&) = default;

		static  Glfw* m_Instance;

		static  void  CB_FrameBuffSize(GLFWwindow* window, int width, int height);
		
		void    GenWindow(size_t width = 800, size_t height = 600, const std::string & title = "title");
		void    LoadGL();

		GLFWwindow*		window;

	private:
		Oper::Ptr<Oper::OpQueue>   mRenderEnv;
		Oper::Ptr<Oper::OpQueue>   mForwardProcess;
		Oper::Ptr<Oper::OpQueue>   mPostProcess;
		Oper::Ptr<Oper::OpQueue>   mFrameProcess;

	};
}