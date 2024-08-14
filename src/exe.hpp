#include "top.hpp"

#ifndef dChalGameOfLife_exe_hpp
#define dChalGameOfLife_exe_hpp

#include "gui.hpp"

namespace nExe {
class tOrganism {
public://consdef

	static constexpr auto vAntibioSurviveChance = 3;
	static constexpr auto vAntibioKillingChance = 10;
	static_assert(
		vAntibioSurviveChance < vAntibioKillingChance,
		"survival chance is included in the killing chance;"
		"an organism survives if a random value [1,AntibioKillingChance]"
		" is less than or equal to the AntibioSurviveChance;"
		"an organism dies if a random value [1,AntibioKillingChance]"
		" is more than the AntibioSurviveChance;"
	);

public://codetor

	tOrganism(std::optional<tOrganism> vAncestor = {});

public://getters

	inline auto fGetAliveStepCount() const {
		return this->vAliveStepCount;
	}
	inline auto fGetAliveStepLimit() const {
		return this->vAliveStepLimit;
	}

	inline auto fGetReproStepCount() const {
		return this->vReproStepCount;
	}
	inline auto fGetReproStepLimit() const {
		return this->vReproStepLimit;
	}
	inline auto fGetReproIndex() const {
		return this->vReproIndex;
	}

public://vetters

	inline auto fVetAlive(bool vAliveValue = 1) const {
		return (this->vAliveValue == vAliveValue);
	}

public://actions

	std::optional<tOrganism> fRunStep();

	bool fTryAntibio();

private://actions

	bool fTryDeath();

	std::optional<tOrganism> fTryRepro();

private://vardefs

	//lifetime
	long			 vAliveStepCount;//how many steps since birth?
	const long vAliveStepLimit;//how many steps until death?
	bool			 vAliveValue;		 //are we alive?

	//reproduction
	long			 vReproStepCount;//how many steps since the last reproduction?
	const long vReproStepLimit;//how many steps until the next reproduction?
	const long vReproIndex;		 //the current generation number
};
}//namespace nExe
namespace nExe {
bool fRun(int vArgC, char **vArgV);
bool fTry(int vArgC, char **vArgV);
}//namespace nExe

extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
