#include "exe.hpp"

namespace nExe {
/* tOrganism */
/** codetor **/
tOrganism::tOrganism()
	: vAliveTimeSince(tTimeClock::now())
  , vAliveTimeCount(std::uniform_int_distribution<>(1, 16)(vRandomGen))
	, vReproTimeSince(tTimeClock::now())
	, vReproTimeCount() {
}
/** actions **/
bool tOrganism::fUpd() {
	auto vClockTimePoint = tTimeClock::now();

	auto vReproTimeUntil = this->fGetReproTimeUntil().time_since_epoch();
	if(vReproTimeUntil - vClockTimePoint) {
	}

	auto vAliveTimeUntil = this->fGetAliveTimeUntil();
	if(vAliveTimeUntil > vClockTimePoint) {
	}

	return 1;
}
bool tOrganism::fTryAntibio() {
	return 1;
}
}//namespace nExe
namespace nExe {
bool fRun(int vArgC, char **vArgV) {
	return 1;
}
bool fTry(int vArgC, char **vArgV) {
	fmt::println(stderr, "[exe]=(");
	fmt::println(stderr, "[vArgC]={};", vArgC);
	fmt::println(stderr, "[vArgV][0]={};", vArgV[0]);
	fmt::println(stderr, "[dProjName]={};", dProjName);
	fmt::
		println(stderr, "[dWorkPath]={};", std::filesystem::current_path().c_str());
	nGui::fTry(vArgC, vArgV);
	fmt::println(stderr, ")=[exe]");
	return 1;
}
}//namespace nExe

int main(int vArgC, char **vArgV) {
	if constexpr(0) {
		return nExe::fRun(vArgC, vArgV) ? 0 : 1;
	} else {
		return nExe::fTry(vArgC, vArgV) ? 0 : 1;
	}
}
