#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented {
	class Camera {
	public:
		Camera() { }

		Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up) {
			Set(position, center, up);
		}

		~Camera() { }

		// Update camera
		void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up) {
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance) {
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
			position += dir * distance;
		}

		void TranslateForward(float distance) {
			position += glm::normalize(forward) * distance;
		}

		void TranslateUpward(float distance) {
			position += glm::normalize(up) * distance;
		}

		void TranslateRight(float distance) {
			position += glm::normalize(right) * distance;
		}

		void RotateFirstPerson_OX(float angle) {
			forward = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 0));
			up = glm::normalize(glm::cross(right, forward));
		}

		void RotateFirstPerson_OY(float angle) {
			forward = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(forward, 0));
			right = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(right, 0));
			up = glm::normalize(glm::cross(right, forward));
		}

		void RotateFirstPerson_OZ(float angle) {
			right = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 0));
			up = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(up, 0));
			forward = glm::cross(up, right);
		}

		void RotateThirdPerson_OX(float angle) {
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle) {
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OZ(float angle) {
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OZ(angle);
			TranslateForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix() {
			// Returns the view matrix
			return glm::lookAt(position, position + forward, up);;
		}

		glm::vec3 GetTargetPosition() {
			return position + forward * distanceToTarget;
		}

		void setDistanceToTarget(float distance) {
			distanceToTarget = distance;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}   // namespace implemented
