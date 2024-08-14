#include "exe.hpp"

namespace nExe {
/* tOrganism */
/** codetor **/
tOrganism::tOrganism(std::optional<tOrganism> vAncestor)
	: vAliveStepCount()
	, vAliveStepLimit(
			vAncestor ? vAncestor->vAliveStepLimit
								: std::uniform_int_distribution<>(2, 8)(vRandomGen)
		)
	, vAliveValue(1)
	, vReproStepCount()
	, vReproStepLimit(
			vAncestor ? vAncestor->vReproStepLimit
								: std::uniform_int_distribution<>(1, 4)(vRandomGen)
		)
	, vReproIndex(vAncestor ? (vAncestor->vReproIndex + 1) : 0) {
}
/** actions **/
std::optional<tOrganism> tOrganism::fRunStep() {
	if(this->fVetAlive(0)) {
		throw std::runtime_error("running step for a dead organism");
	}
	std::optional<tOrganism> vChild;
	if(this->vReproStepCount < this->vReproStepLimit) {
		this->vReproStepCount += 1;
	} else {
		vChild.emplace(this->fTryRepro());
	}
	if(this->vAliveStepCount < this->vAliveStepLimit) {
		this->vAliveStepCount += 1;
	} else {
		this->fTryDeath();
	}
	return vChild;
}
bool tOrganism::fTryAntibio() {
	if(this->fVetAlive(0)) {
		throw std::runtime_error("trying antibio on a dead organism");
	}
	auto vAntibioSurviveChance = std::
		uniform_int_distribution<>(1, vAntibioKillingChance)(vRandomGen);
	if(vAntibioSurviveChance > this->vAntibioSurviveChance) {
		return this->fTryDeath();
	} else {
		return 0;
	}
}
bool tOrganism::fTryDeath() {
	if(this->fVetAlive(0)) {
		throw std::runtime_error("trying death on a dead organism");
	}
	this->vAliveValue = 0;
	return 1;
}
std::optional<tOrganism> tOrganism::fTryRepro() {
	if(this->fVetAlive(0)) {
		throw std::runtime_error("trying death on a dead organism");
	}
	return tOrganism(*this);
}
}//namespace nExe
namespace nExe {
bool fRun(int vArgC, char **vArgV) {
	auto vStepIndex = 0;
	fmt::println(stdout, "Simulation is being started...");
	auto vCountUse = 0;
	auto vCountMin = 1;
	auto vCountMax = 20;
	while(vCountMin > vCountUse || vCountMax < vCountUse) {
		fmt::println(stdout, "We cannot create {} organisms.", vCountUse);
		fmt::println(
			stdout,
			"Type the amount of organisms from {} to {} (inclusively):",
			vCountMin,
			vCountMax
		);
		try {
			std::string vInput;
			std::size_t vReadP;
			std::getline(std::cin, vInput);
			auto vCountGot = std::stoi(vInput, &vReadP);
			if(vReadP < vInput.length()) {
				throw std::invalid_argument("Extra characters in the input!");
			}
			vCountUse = vCountGot;
		} catch(const std::exception &vError) {
			fmt::println(stdout, "Read failure! Still having {};", vCountUse);
		}
	}
	auto vArray			 = std::vector<tOrganism>(vCountUse);
	auto vChildArray = std::vector<std::optional<tOrganism>>();
	auto vDeathCount = 0;
	bool vRunFlag		 = 1;
	while(vRunFlag) {
		fmt::println(stdout, "Step index: {};", vStepIndex);
		fmt::println(stdout, "Alive count: {};", vArray.size());
		fmt::println(stdout, "Death count: {};", vDeathCount);
		fmt::println(stdout, "");
		char vInput = '\0';
		fmt::println(
			stdout,
			"Choose your action:"
			"\n- 's' to step forward in time;"
			"\n- 'a' to use AntiBiotics;"
			"\n- 'd' to display information about each organism;"
			"- 'q' to quit;"
		);
		try {
			std::cin >> vInput;
		} catch(const std::exception &vError) {
			fmt::println(stdout, "Read failure! Not gonna do anything;");
		}
		fmt::println(stdout, "");
		//would like to use an unordered_map of callbacks here
		//but this would be an overkill for a few actions
		//especially for these callbacks to control the context
		switch(vInput) {
		case 's': {
			fmt::println("Step forward is being done.");
			for(auto &vEntry: vArray) {
				vChildArray.push_back(vEntry.fRunStep());
			}
			fmt::println("Step forward has been done.");
			vStepIndex++;
		} break;
		case 'a': {
			fmt::println("Using AntiBiotics:");
			for(auto &vEntry: vArray) {
				vEntry.fTryAntibio();
			}
		} break;
		case 'd': {
			fmt::println("Organism status display:");
			for(auto vIndex = 0; vIndex < vArray.size(); vIndex++) {
				const auto &vEntry = vArray[vIndex];
				const auto	vOrder = vIndex + 1;
				fmt::println(
					"- [{}]"
					" Alive: {};"
					" AliveStepCount: {};"
					" AliveStepLimit: {};"
					" ReproStepCount: {};"
					" ReproStepLimit: {};"
					" ReproIndex: {};"
					"",
					vOrder,
					vEntry.fVetAlive(1),
					vEntry.fGetAliveStepCount(),
					vEntry.fGetAliveStepLimit(),
					vEntry.fGetReproStepCount(),
					vEntry.fGetReproStepLimit(),
					vEntry.fGetReproIndex()
				);
			}
		} break;
		case 'q': {
			fmt::println(stdout, "Simulation is being finished...");
			vRunFlag = 0;
		} break;
		default: {
			fmt::println(stdout, "Unknown action ({})", vInput);
		} break;
		}
		fmt::println(stdout, "");
		//insert children into the main array
		for(auto &vChildEntry: vChildArray) {
			if(vChildEntry) {
				vArray.push_back(*vChildEntry);
			}
		}
		vChildArray.clear();
		//keeping alive, leaving the dead
		auto vAliveCount = 0;
		for(auto &vEntry: vArray) {
			vAliveCount += vEntry.fVetAlive(1);
		}
		if(vAliveCount != vArray.size()) {
			decltype(vArray) vAliveArray;
			vAliveArray.reserve(vAliveCount);
			for(auto &vEntry: vArray) {
				if(vEntry.fVetAlive(1)) {
					vAliveArray.push_back(vEntry);
				} else {
					vDeathCount++;
				}
			}
			vArray = std::move(vAliveArray);
		}
	}
	fmt::println(stdout, "Simulation has been finished...");
	return 1;
}
bool fTry(int vArgC, char **vArgV) {
	fmt::println(stderr, "[exe]=(");
	fLogErr("vArgC", "{};", vArgC);
	fLogErr("vArgV[0]", "\"{}\";", vArgV[0]);
	fLogErr("dProjName", "\"{}\";", dProjName);
	fLogErr("dWorkPath", "\"{}\";", std::filesystem::current_path().c_str());
	nGui::fTry(vArgC, vArgV);
	fmt::println(stderr, ")=[exe]");
	return 1;
}
}//namespace nExe

int main(int vArgC, char **vArgV) {
	try {
		if constexpr(1) {
			return nExe::fRun(vArgC, vArgV) ? 0 : 1;
		} else {
			return nExe::fTry(vArgC, vArgV) ? 0 : 1;
		}
	} catch(const std::exception &vError) {
		fLogErr("[vError]", "\"{}\";", vError.what());
	}

	return 1;
}
