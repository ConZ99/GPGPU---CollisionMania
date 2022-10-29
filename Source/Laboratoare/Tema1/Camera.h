#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Camera
{
	class Camera
	{
	public:
		Camera()
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);
			distanceToTarget = 2;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			glm::vec3 move = glm::normalize(forward);
		}

		void TranslateForward(float distance)
		{
			position += glm::normalize(forward) * distance;
		}

		void TranslateUpword(float distance)
		{
			position += glm::normalize(up) * distance;
		}

		void TranslateRight(float distance)
		{
			position += glm::normalize(right) * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			glm::vec4 vec1 = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
			forward = glm::normalize(glm::vec3(vec1));

			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OY(float angle)
		{
			glm::vec4 vec1 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
			forward = glm::normalize(glm::vec3(vec1));

			glm::vec4 vec2 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
			right = glm::normalize(glm::vec3(vec2));

			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OZ(float angle)
		{
			glm::vec4 vec1 = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
			right = glm::normalize(glm::vec3(vec1));

			up = glm::cross(right, forward);
		}

		void RotateThirdPerson_OX(float angle)
		{
			MoveForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			MoveForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			MoveForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			MoveForward(-distanceToTarget);
		}

		void RotateThirdPerson_OZ(float angle)
		{
			MoveForward(distanceToTarget);
			RotateFirstPerson_OZ(angle);
			MoveForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}