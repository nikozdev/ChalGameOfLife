#include "app.hpp"

#include "sim.hpp"
#include "gui.hpp"

namespace nApp {

	//tApp{

	//codetor

	tApp::tApp(int vArgC, char **vArgV)
		: QApplication(vArgC, vArgV), vWindow{new nGui::tAppWindow()} {
		this->setStyleSheet(
			"QWidget {"
			" background: black;"
			" alternate-background-color: dark-gray;"
			" accent-color: light-gray;"
			" color: cyan;"
			" placeholder-text-color: dark-cyan;"
			" selection-color: light-cyan;"
			"}"
		);
	}

	tApp::~tApp() {
		delete this->vWindow;
    disconnect();
	}

	//actions

	void tApp::fRunSim(unsigned vSimBeingCount) {
		for(size_t vIndex = 0; vIndex < vSimBeingCount; vIndex++) {
			this->fNewSimBeing();
		}
		emit this->sRunSimCall();
	}

	void tApp::fRunEventKeyPress(QKeyEvent *vQKeyEvent) {
		auto vQKeyBytes = vQKeyEvent->text().toLower().toUtf8().constData();
		auto vQKeyValue = vQKeyBytes[0];
		switch(vQKeyValue) {
		case 'a': {
			this->vWindow->vSimWindow->vOutput->setText(
				fmt::format(
					"[{}]: "
					"Using antibiotics...",
					vQKeyValue
				)
					.c_str()
			);
			emit this->sTryAntibioCall();
		} break;
		case 'q': {
			this->vWindow->vSimWindow->vOutput->setText(
				fmt::format(
					"[{}]: "
					"Bye-bye!",
					vQKeyValue
				)
					.c_str()
			);
			fLogErr("TryKeyPress", "Quit is being done;");
			this->quit();
			fLogErr("TryKeyPress", "Quit has been done;");
		} break;
		default: {
			if(vQKeyEvent->text()[0].isLetterOrNumber()) {
				this->vWindow->vSimWindow->vOutput
					->setText(fmt::format("[{}]: Nothing to do;", vQKeyValue).c_str());
			}
		} break;
		}
	}

	std::shared_ptr<nSim::tBeing> tApp::fNewSimBeing(nSim::tBeing *vAncestor) {
		auto vBeing = std::make_shared<nSim::tBeing>(vAncestor);
		connect(
			this, &tApp::sTryAntibioCall, vBeing.get(), &nSim::tBeing::sTryAntibioSlot
		);
		connect(vBeing.get(), &nSim::tBeing::sBirthCall, this, [this]() {
			fLogErr("BirthCall", "");
			auto vBeing = static_cast<nSim::tBeing *>(QObject::sender());
			this->fNewSimBeing(vBeing);
		});

		auto &vLayout = this->vWindow->vSimWindow->vReport->vScroll->vWidget->vLayout;
		auto *vLabel = new QLabel("simulation being;");
		vLayout->addWidget(vLabel, 1);
		auto *vLabelItem = vLayout->itemAt(vLayout->count() - 1);

		connect(vBeing.get(), &nSim::tBeing::sAliveCall, this, [vBeing, vLabel]() {
			auto vTimerPoint = std::chrono::duration_cast<nSim::tBeing::tTimerPoint>(
				nSim::tBeing::tTimerClock::now().time_since_epoch()
			);
			vLabel->setText(
				fmt::format(
					"generation={};"
					"\n- deathAfter={}; aliveSince={}; aliveLimit={};"
					"\n- reproAfter={}; reproSince={}; reproLimit={};",
					vBeing->vReproIndex,
					vBeing->vAliveTimerLimit - (vTimerPoint - vBeing->vAliveTimerSince),
					vBeing->vAliveTimerSince,
					vBeing->vAliveTimerLimit,
					vBeing->vReproTimerLimit - (vTimerPoint - vBeing->vReproTimerSince),
					vBeing->vReproTimerSince,
					vBeing->vReproTimerLimit
				)
					.c_str()
			);
		});
		connect(vBeing.get(), &nSim::tBeing::sDeathCall, this, [vBeing, vLabelItem]() {
			fLogErr("DeathCall", "");
			auto vWidget = vLabelItem->widget();
			if(vWidget) {
        fLogErr("DeathCall", "widget is being deleted;");
				vWidget->setParent(nullptr);
				delete vWidget;
        fLogErr("DeathCall", "widget has been deleted;");
			}
      //fLogErr("DeathCall", "item is being deleted;");
			//delete vLabelItem;
      //fLogErr("DeathCall", "item has been deleted;");
			vBeing->requestInterruption();
			vBeing->wait();
		});
		vBeing->start();
		connect(this, &tApp::aboutToQuit, this, [vBeing]() {
			fLogErr("AboutToQuit", "");
			vBeing->requestInterruption();
			vBeing->wait();
		});
		return vBeing;
	}

	//}tApp

}//namespace nApp
