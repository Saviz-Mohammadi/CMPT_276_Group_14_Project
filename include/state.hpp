#ifndef STATE_H
#define STATE_H

class StateManager;

class State
{
public:
    explicit State();
    virtual ~State();
    
public:
    void init(StateManager* state_manager);

public:
    virtual void onEnter() = 0;
    virtual void onProcess() = 0;
    virtual void onExit() = 0;
    
protected:
    StateManager* m_state_manager;
};

#endif // STATE_H
