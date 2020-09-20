#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include  <glm/gtc/matrix_transform.hpp>

namespace OpenGL {
	class Camera
	{
	public:
		enum ENUM_Movement {
			MOVE_FORWARD,
			MOVE_BACKWARD,
			MOVE_LEFT,
			MOVE_RIGHT,
			MOVE_UP,
			MOVE_DOWN
		};
		enum ENUM_Projection {
			PROJECTION_PERSEPCTIVE,
			PROJECTION_ORTHO
		};

		// Constructor with vectors
		Camera(double rationWH = RATIO_WH, double moveSpeed = 10.0f, double rotateSpeed = 0.02f, glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f), double nearPlane = NEAR_PLANE, double farPlane = FAR_PLANE, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), double yaw = YAW, double pitch = PITCH, ENUM_Projection projectionMode = PROJECTION_MODE);

		glm::mat4 GetViewMatrix();

		// Returns the projection matrix calculated using zoom, ratioWH, nearPlane, farPlane
		glm::mat4 GetProjectionMatrix();

		void SetPerspective();

		void SetOrtho();
		void SetZoom(double zoom);
		void SetFront(const glm::vec3& camerafront);

		glm::vec3 & GetPos();
		glm::vec3 & GetFront();

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(ENUM_Movement direction, double deltaTime);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(double xPos, double yPos, bool constrainPitch = true);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(double yoffset);


	private:

		void updateCameraVectors();

		//default camera value
		static const double RATIO_WH;
		static const double NEAR_PLANE;
		static const double FAR_PLANE;
		static const double YAW;
		static const double PITCH;
		static const double ZOOM;
		static const ENUM_Projection PROJECTION_MODE;



	private:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		// Euler  angles
		double Yaw;
		double Pitch;

		//camera options
		double MovementSpeed;
		double MouseSensitivity;
		double Zoom;
		double rationWH;
		double nearPlane;
		double farPlane;
		ENUM_Projection projectionMode;
	};
};