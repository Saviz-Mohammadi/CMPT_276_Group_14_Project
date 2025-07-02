#ifndef VESSEL_MANAGEMENT_STATE_HPP
#define VESSEL_MANAGEMENT_STATE_HPP

#include "state.hpp"

class VesselManagementState : public State
{
public:
    explicit VesselManagementState();
    ~VesselManagementState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // VESSEL_MANAGEMENT_STATE_HPP
