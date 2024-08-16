#include "sim.hpp"

namespace nSim {

	//tBeing{

	//codetor

	tBeing::tBeing(tBeing *vAncestor)
		: QThread(QApplication::instance())
		, vAliveTimerSince{std::chrono::duration_cast<
				tTimerPoint>(tTimerClock::now().time_since_epoch())}
		, vAliveTimerLimit{vAncestor ? vAncestor->vAliveTimerLimit : std::chrono::seconds(std::uniform_int_distribution<unsigned>(vAliveTimerLimitMin, vAliveTimerLimitMax)(vRandomGen))}
		, vReproTimerSince{vAliveTimerSince}
		, vReproTimerLimit{vAncestor ? vAncestor->vReproTimerLimit : std::chrono::seconds(std::uniform_int_distribution<unsigned>(vReproTimerLimitMin, vReproTimerLimitMax)(vRandomGen))}
		, vReproIndex{vAncestor ? vAncestor->vReproIndex + 1 : 0}
		, vMutex{} {
	}

	tBeing::~tBeing() {
    this->requestInterruption();
    this->wait();
		this->disconnect();
    fLogErr("~tBeing()", "has been done");
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
		auto vAliveRange = std::uniform_int_distribution<
			unsigned>(1, this->vAntibioAliveChance + this->vAntibioDeathChance);
		if(vAliveRange(vRandomGen)) {
			this->requestInterruption();
		}
	}

  void tBeing::sAboutToQuitSlot() {
    this->requestInterruption();
    this->wait();
  }

	//}tBeing

}//namespace nSim
