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
		auto vQKeyBytes = vQKeyEvent->text().toLower().toUtf8();
		auto vQKeyValue = vQKeyBytes[0];
		switch(vQKeyValue) {
		case 'a': {
      if constexpr(0) {
        this->vWindow->vSimWindow->vOutput->setText(
          fmt::format(
            "[{}]: "
            "Using antibiotics...",
            vQKeyValue
          )
          .c_str()
        );
      } else {
        this->vWindow->vSimWindow->vOutput->setText(
          fmt::format(
            "[{}]: "
            "Применяем антибиотик: случайное количество микро-организмов погибает.",
            vQKeyValue
          )
          .c_str()
        );
      }
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
			this->quit();
		} break;
		default: {
      if(vQKeyEvent->text()[0].isLetterOrNumber()) {
        if constexpr(0) {
          this->vWindow->vSimWindow->vOutput
          ->setText(fmt::format("[{}]: Nothing to do;", vQKeyValue).c_str());
        } else {
          this->vWindow->vSimWindow->vOutput
          ->setText(fmt::format("[{}]: Делать нечего;", vQKeyValue).c_str());
        }
      }
		} break;
		}
	}

	nSim::tBeing *tApp::fNewSimBeing(nSim::tBeing *vAncestor) {
		auto vBeing = new nSim::tBeing(vAncestor);
		connect(this, &tApp::sTryAntibioCall, vBeing, &nSim::tBeing::sTryAntibioSlot);
		connect(vBeing, &nSim::tBeing::sBirthCall, this, [this]() {
			auto vBeing = static_cast<nSim::tBeing *>(QObject::sender());
			this->fNewSimBeing(vBeing);
		});

		auto &vLayout = this->vWindow->vSimWindow->vReport->vScroll->vWidget->vLayout;
		auto *vLabel = new QLabel("simulation being;");
		vLayout->addWidget(vLabel, 1);
		auto *vLabelItem = vLayout->itemAt(vLayout->count() - 1);

		connect(vBeing, &nSim::tBeing::sAliveCall, this, [vBeing, vLabel]() {
			auto vTimerPoint = std::chrono::duration_cast<nSim::tBeing::tTimerPoint>(
				nSim::tBeing::tTimerClock::now().time_since_epoch()
			);
      if constexpr(0) {
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
      } else {
        vLabel->setText(
          fmt::format(
            "поколение: {};"
            "\n- погибает через: {};"
            "\n- размножение через: {};",
            vBeing->vReproIndex,
            vBeing->vAliveTimerLimit - (vTimerPoint - vBeing->vAliveTimerSince),
            vBeing->vReproTimerLimit - (vTimerPoint - vBeing->vReproTimerSince)
          )
          .c_str()
        );
      }
		});
		connect(vBeing, &nSim::tBeing::sDeathCall, this, [vBeing, vLabel, vLayout]() {
			vLayout->removeWidget(vLabel);
			delete vLabel;
			vBeing->wait();
			vBeing->setParent(nullptr);
			delete vBeing;
		});
		vBeing->start();
		connect(this, &tApp::aboutToQuit, vBeing, &nSim::tBeing::sAboutToQuitSlot);
		return vBeing;
	}

	//}tApp

}//namespace nApp
