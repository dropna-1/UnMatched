#pragma once
#include "raylib.h"
#include <vector>
#include <string>

struct HelpPage {
    const char* title;
    std::vector<const char*> lines;
};

class HelpView {
public:
    HelpView();

    void Open();
    void Close();
    bool IsOpen() const;

    void Update();
    void Draw(Font font);

private:
    bool open = false;
    int pageIndex = 0;
    std::vector<HelpPage> pages;

    Rectangle btnClose{};
    Rectangle btnPrev{};
    Rectangle btnNext{};
};