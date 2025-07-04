#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

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

#endif // MAIN_MENU_STATE_HPP
