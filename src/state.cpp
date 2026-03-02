#include "state.h"

State* State::instance = nullptr;

State::State()
{
}

State* State::GetInstance()
{
	if(instance == nullptr)
		instance = new State();
	return instance;
}

State& g = *State::GetInstance();
