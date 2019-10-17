#include <glm/glm.hpp>

class Camera {

	const glm::vec3 UP;
	glm::vec2 oldMousePosition;

	public:
		glm::vec3 viewDirection;	
		Camera();
		void mouseUpdate( const glm::vec2& newMousePosition);
		glm::mat4 getWorlToViewMatrix() const;
		glm::vec3 position;

		void moveForward();
		void moveBackward();
		void strafeLeft();
		void strafeRight();
		void moveUp();
		void moveDown();
};