#pragma once

class ScreenManager;

class IScreen {
public:
    virtual ~IScreen() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void HandleInput() = 0;
protected:
    ScreenManager* manager = nullptr;
};