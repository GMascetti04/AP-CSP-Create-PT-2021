#include "xpch.h"
#include "Components.h"   
#include "Engine/Core/Application.h"


std::tuple<float, float> Script::getMousePosition()
{
    CameraController* camController = m_entity.getScene()->getCameraController();
    float xPos = Input::percentMouseX() * camController->getAspectRatio() * camController->getCamHeight() + camController->getCamera()->getPosition().x - camController->getAspectRatio() * camController->getCamHeight()/ 2.0f;
    float yPos = Input::percentMouseY() * camController->getCamHeight() + camController->getCamera()->getPosition().y - camController->getCamHeight() /2.0f;
    return std::tuple<float, float>(xPos, yPos);
}

std::tuple<int, int> Script::worldToScreenSpace(float x, float y)
{
    CameraController* camController = m_entity.getScene()->getCameraController();
    float xPos = (x + ((camController->getAspectRatio() * camController->getCamHeight())/2.0f) - camController->getCamera()->getPosition().x) / (float)(camController->getAspectRatio() * camController->getCamHeight()) * application::getWindow()->getWidth();
    float yPos = application::getWindow()->getHeight() - ((y + (camController->getCamHeight()/2.0f) - camController->getCamera()->getPosition().y) / (float)(camController->getCamHeight()) * application::getWindow()->getHeight());
    return std::tuple<int, int>((int)xPos,(int)yPos);
}


