class Fsm
{
public:
	Fsm();
	enum State { Registration, Syncronization, SyncroComplete, Trimming, TrimmingComplete, Running, Freeze, Crash };
	State getCurrState();
	bool nextState();
	bool crash();

private:
	State currState;

};