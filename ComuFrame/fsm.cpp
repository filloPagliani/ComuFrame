#include "fsm.h"

Fsm::Fsm() {
	this->currState = State::Registration;
}

Fsm::State Fsm::getCurrState() {
	return this->currState;
}

bool Fsm::crash() {
	if ((this->currState == Running) || (this->currState == Trimming)) {
		this->currState = Crash;
		return true;
	}
	else {
		return false;
	}
}

bool Fsm::nextState() {
	switch (this->currState)
	{
	case State::Registration:
		this->currState = Syncronization;
		return true;
	case State::Syncronization:
		this->currState = SyncroComplete;
		return true;
	case State::SyncroComplete:
		this->currState = Trimming;
		return true;
	case State::Trimming:
		this->currState = TrimmingComplete;
		return true;
	case State::TrimmingComplete:
		this->currState = Running;
		return true;
	case State::Running:
		this->currState = Freeze;
		return true;
	default:
		return false;
	}
}