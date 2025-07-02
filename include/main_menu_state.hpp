#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "state.hpp"

class MainMenuState : public State
{
public:
    explicit MainMenuState();
    ~MainMenuState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // MAIN_MENU_STATE_H
