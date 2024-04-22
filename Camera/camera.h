#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "..\Graphics\window.h"

class Camera
{
private:

    bool isJumping = false;
    bool finishAnimation = false;

    static constexpr float cameraHeight = 18.f;


    glm::vec3 cameraPosition;
    glm::vec3 cameraViewDirection;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;


public:
    Camera();
    Camera(glm::vec3 cameraPosition);
    Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp);
    ~Camera() = default;

    bool getJumpingState() const noexcept;

    glm::vec3 getCameraUp();
    glm::mat4 getViewMatrix();
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraViewDirection();


    void keyboardMoveUp(float cameraSpeed);
    void keyboardMoveDown(float cameraSpeed);
    void keyboardMoveBack(float cameraSpeed);
    void keyboardMoveLeft(float cameraSpeed);
    void keyboardMoveFront(float cameraSpeed);
    void keyboardMoveRight(float cameraSpeed);
    void setCameraX(const float poz) noexcept;
    void setCameraY(const float poz) noexcept;
    void setCameraZ(const float poz) noexcept;
    void setJumping(const bool jumping) noexcept;
    void setCameraPosition(const glm::vec3& newDir) noexcept;
    void setCameraViewDirection(const glm::vec3& newDir) noexcept;


    constexpr float getCameraHeight() const noexcept
    {
        return this->cameraHeight;
    }

    /*friend std::ostream& operator<<(std::ostream& os, const Camera& obj);*/
};
