#include "top.hpp"

#ifndef dChalGameOfLife_exe_hpp
#define dChalGameOfLife_exe_hpp

#include "gui.hpp"

namespace nExe {
class tOrganism {
public://codetor

	tOrganism();

public://getters

	inline auto fGetAliveTimeSince() {
		return this->vAliveTimeSince;
	}
	inline auto fGetAliveTimeCount() {
		return this->vAliveTimeCount;
	}
	inline auto fGetAliveTimeUntil() {
		return this->vAliveTimeSince + this->vAliveTimeCount;
	}

	inline auto fGetReproTimeSince() {
		return this->vReproTimeSince;
	}
	inline auto fGetReproTimeCount() {
		return this->vReproTimeCount;
	}
	inline auto fGetReproTimeUntil() {
		return this->vReproTimeSince + this->vReproTimeCount;
	}

public://actions

	bool fUpd();

	bool fTryAntibio();

private://actions

	bool fTryDiying();
	bool fTryRepro();

private://vardefs

	//lifetime
	tTimePoint vAliveTimeSince;//the birthday of my self
	tTimeCount vAliveTimeCount;//the death of my self?

	//reproduction
	tTimePoint vReproTimeSince;//this is our birthday by default
	tTimeCount vReproTimeCount;//the birth of my next child?
};
}//namespace nExe
namespace nExe {
bool fRun(int vArgC, char **vArgV);
bool fTry(int vArgC, char **vArgV);
}//namespace nExe

extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
