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
		Camera(float rationWH = RATIO_WH, float moveSpeed = 10.0f, float rotateSpeed = 0.02f, glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f), float nearPlane = NEAR_PLANE, float farPlane = FAR_PLANE, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, ENUM_Projection projectionMode = PROJECTION_MODE);

		glm::mat4 GetViewMatrix();

		// Returns the projection matrix calculated using zoom, ratioWH, nearPlane, farPlane
		glm::mat4 GetProjectionMatrix();

		void SetPerspective();

		void SetOrtho();
		void SetFront(const glm::vec3& camerafront);

		glm::vec3 & GetPos();
		glm::vec3 & GetFront();

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(ENUM_Movement direction, float deltaTime);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset);


	private:

		void updateCameraVectors();

		//default camera value
		static const float RATIO_WH;
		static const float NEAR_PLANE;
		static const float FAR_PLANE;
		static const float YAW;
		static const float PITCH;
		static const float ZOOM;
		static const ENUM_Projection PROJECTION_MODE;



	private:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		// Euler  angles
		float Yaw;
		float Pitch;

		//camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
		float rationWH;
		float nearPlane;
		float farPlane;
		ENUM_Projection projectionMode;
	};
};