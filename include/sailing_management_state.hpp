#ifndef SAILING_MANAGEMENT_STATE_HPP
#define SAILING_MANAGEMENT_STATE_HPP

#include "state.hpp"

class SailingManagementState : public State
{
public:
    explicit SailingManagementState();
    ~SailingManagementState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // SAILING_MANAGEMENT_STATE_HPP
