#include "Fsm.h"

Fsm::Fsm() {
	this->currState = State::Registration;
}

std::string Fsm::strState(State state) {
	switch (state){
	case Registration: return "registration";
	case Syncronization: return "syncronization";
	case SyncroComplete: return "syncroComplete";
	case Trimming: return "srimming";
	case TrimmingComplete: return "trimmingComplete";
	case Running: return "tunning";
	case Freeze: return "treeze";
	case Crash: return "crash";
	default: return "ERROR";
	}
}

Fsm::State Fsm::getCurrState() {
	return this->currState;
}

std::string Fsm::toString() {
	switch (this->currState)
	{
	case Registration:
		return "registration";
	case Syncronization:
		return "syncronization";
	case SyncroComplete:
		return "syncroComplete";
	case Trimming:
		return "trimming";
	case TrimmingComplete:
		return "trimmingComplete";
	case Running:
		return "running";
	case Freeze:
		return "freeze";
	case Crash:
		return "crash";
	default:
		return "";
	}
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

