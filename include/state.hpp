#ifndef STATE_HPP
#define STATE_HPP

class StateManager;
class Database;

class State
{
public:
    explicit State();
    virtual ~State();
    
public:
    void init(StateManager* state_manager, Database* database);

public:
    virtual void onEnter() = 0;
    virtual void onProcess() = 0;
    virtual void onExit() = 0;
    
protected:
    StateManager* m_state_manager;
    Database* m_database;
};

#endif // STATE_HPP
