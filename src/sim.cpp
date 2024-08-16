#include "sim.hpp"

namespace nSim {

	//tBeing{

	//codetor

	tBeing::tBeing(tBeing *vAncestor)
		: QThread(nullptr)
		, vAliveTimerSince{std::chrono::duration_cast<
				tTimerPoint>(tTimerClock::now().time_since_epoch())}
		, vAliveTimerLimit{vAncestor ? vAncestor->vAliveTimerLimit : std::chrono::seconds(std::uniform_int_distribution<unsigned>(vAliveTimerLimitMin, vAliveTimerLimitMax)(vRandomGen))}
		, vReproTimerSince{vAliveTimerSince}
		, vReproTimerLimit{vAncestor ? vAncestor->vReproTimerLimit : std::chrono::seconds(std::uniform_int_distribution<unsigned>(vReproTimerLimitMin, vReproTimerLimitMax)(vRandomGen))}
		, vReproIndex{vAncestor ? vAncestor->vReproIndex + 1 : 0}
		, vMutex{} {
		fLogErr(
			"nSim::tBeing::tBeing",
			"{}; {}; {}; {}; {}; {};",
			(void *)vAncestor,
			vAliveTimerSince,
			vAliveTimerLimit,
			vReproTimerSince,
			vReproTimerLimit,
			vReproIndex
		);
	}

	tBeing::~tBeing() {
		this->disconnect();
	}

	//actions

	void tBeing::run() {
		while(!this->isInterruptionRequested()) {
			QMutexLocker vMutexLocker(&this->vMutex);
			this->msleep(1'000);

			auto vTimerPoint = std::chrono::duration_cast<
				tTimerPoint>(tTimerClock::now().time_since_epoch());
			if(vTimerPoint > (this->vAliveTimerSince + this->vAliveTimerLimit)) {
				this->requestInterruption();
			}
			if(vTimerPoint > (this->vReproTimerSince + this->vReproTimerLimit)) {
				this->vReproTimerSince = vTimerPoint;
				emit this->sBirthCall();
			}
			emit this->sAliveCall();
		}
		emit this->sDeathCall();
	}

	//slots

	void tBeing::sTryAntibioSlot() {
		QMutexLocker vMutexLocker(&this->vMutex);
    fLogErr("sTryAntibioSlot", "");

		auto vAliveRange = std::uniform_int_distribution<
			unsigned>(1, this->vAntibioAliveChance + this->vAntibioDeathChance);
		if(vAliveRange(vRandomGen)) {
			emit this->sDeathCall();
		}
	}

	//}tBeing

}//namespace nSim
