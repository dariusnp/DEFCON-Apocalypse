#include "camera.h"

Camera::Camera()
{
    this->cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);

}

Camera::Camera(glm::vec3 cameraPosition)
{
    this->cameraPosition = cameraPosition;
    this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);

}


Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
    this->cameraPosition.x = cameraPosition.x;
    this->cameraPosition.y = cameraHeight;
    this->cameraPosition.z = cameraPosition.z;
    this->cameraViewDirection = cameraViewDirection;
    this->cameraUp = cameraUp;
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
    cameraPosition.x += cameraViewDirection.x * cameraSpeed;
    cameraPosition.z += cameraViewDirection.z * cameraSpeed;

    if (!isJumping)
        cameraPosition.y = cameraHeight;
}


void Camera::keyboardMoveBack(float cameraSpeed)
{
    cameraPosition.x -= cameraViewDirection.x * cameraSpeed;
    cameraPosition.z -= cameraViewDirection.z * cameraSpeed;

    

    if (!isJumping)
        cameraPosition.y = cameraHeight;

}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
    cameraPosition.x -= cameraRight.x * cameraSpeed;
    cameraPosition.z -= cameraRight.z * cameraSpeed;


    if (!isJumping)
        cameraPosition.y = cameraHeight;

}

void Camera::keyboardMoveRight(float cameraSpeed)
{
    cameraPosition.x += cameraRight.x * cameraSpeed;
    cameraPosition.z += cameraRight.z * cameraSpeed;


    if (!isJumping)
        cameraPosition.y = cameraHeight;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
    cameraPosition += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
    cameraPosition -= cameraUp * cameraSpeed;
}

void Camera::setCameraViewDirection(const glm::vec3& newDir) noexcept
{
    this->cameraViewDirection = newDir;
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::setCameraPosition(const glm::vec3& newDir) noexcept
{
    this->cameraPosition = newDir;
}

void Camera::setCameraX(const float poz) noexcept
{
    this->cameraPosition.x = poz;
}

void Camera::setCameraY(const float poz) noexcept
{
    this->cameraPosition.y = poz;
}

void Camera::setCameraZ(const float poz) noexcept
{
    this->cameraPosition.z = poz;
}

void Camera::setJumping(const bool jumping) noexcept
{
    this->isJumping = jumping;
}

bool Camera::getJumpingState() const noexcept
{
    return isJumping;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
    return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
    return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
    return cameraUp;
}


//std::ostream& operator<<(std::ostream& os, const Camera& obj) {
//    os << "Camera Position(x = " << obj.cameraPosition.x << ", y = " << obj.cameraPosition.y << ", z = " << obj.cameraPosition.z << ")";
//    return os;
//}