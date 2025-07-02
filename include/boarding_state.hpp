#ifndef BOARDING_STATE_HPP
#define BOARDING_STATE_HPP

#include "state.hpp"

class BoardingState : public State
{
public:
    explicit BoardingState();
    ~BoardingState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // BOARDING_STATE_HPP
