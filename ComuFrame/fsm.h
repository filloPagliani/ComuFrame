#pragma once
#include <stdio.h>
#include <string>

class Fsm
{
public:
	Fsm();
	enum State { Registration, Syncronization, SyncroComplete, Trimming, TrimmingComplete, Running, Freeze, Crash };
	State getCurrState();
	bool nextState();
	bool crash();
	std::string toString();


private:
	State currState;

};