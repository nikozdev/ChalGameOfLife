#include "exe.hpp"

namespace {//simulation

	//{

	///codetor

	tSimBeing::tSimBeing(std::optional<tSimBeing> vAncestor)
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

	///actions

	std::optional<tSimBeing> tSimBeing::fRunStep() {
		if(this->fVetAlive(0)) {
			throw std::runtime_error("running step for a dead organism");
		}
		std::optional<tSimBeing> vChild;
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

	bool tSimBeing::fTryAntibio() {
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

	bool tSimBeing::fTryDeath() {
		if(this->fVetAlive(0)) {
			throw std::runtime_error("trying death on a dead SimBeing");
		}
		this->vAliveValue = 0;
		return 1;
	}

	std::optional<tSimBeing> tSimBeing::fTryRepro() {
		if(this->fVetAlive(0)) {
			throw std::runtime_error("trying death on a dead organism");
		}
		return tSimBeing(*this);
	}

	//}

}

namespace {//system

	//{

	///codetor

	tApp::tApp(int vArgC, char **vArgV)
		: QApplication(vArgC, vArgV)
		, vWin{std::make_shared<tWin>()}
		, vSimStepIndex(0) {
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
	}//ctor

	///actions

	bool tApp::fTryEventKeyPress(QKeyEvent *vQKeyEvent) {
		auto vQKeyBytes = vQKeyEvent->text().toUtf8().constData();
		fLogErr("vQKeyBytes", "{};", vQKeyBytes);
		auto vQKeyValue = vQKeyBytes[0];
		switch(vQKeyValue) {
		case 'a': {
			auto vCount = this->vSimBeingArray.size();
			for(auto vIndex = 0; vIndex < vCount; vIndex++) {
				auto &vEntry = this->vSimBeingArray[vIndex];
				vEntry.fTryAntibio();
			}
		} break;
		case 's': {
			this->vSimStepIndex += 1;
			this->vWin->fGetKeyOutput()
				->setText("Simulation step forward has been done;");
		} break;
		case 'q': {
			this->quit();
		} break;
		default: {
		} break;
		}
		return 1;
	}

	//}

	//{

	///codetor

	tWin::tWin(QWidget *vParentPtr, Qt::WindowFlags vFlags)
		: QWidget(vParentPtr, vFlags)
		, vLayout{std::make_shared<QVBoxLayout>(this)}
		, vKeyOutput{std::make_shared<QTextEdit>("key output", this)}
		, vKeyPrompt{std::make_shared<QTextEdit>("key prompt", this)}
		, vSimStateReport{std::make_shared<QTextEdit>("sim state report", this)}
		, vSimBeingReport{std::make_shared<tSimBeingReport>(this)
			} {
		//vLayout

		this->setLayout(this->vLayout.get());

		//vKeyOutput

		this->vLayout->addWidget(this->vKeyOutput.get(), 2);
		this->vKeyOutput->setReadOnly(1);

    {
      auto vHSep = new QFrame(this);
      this->vLayout->addWidget(vHSep, 1);
      vHSep->setFrameShape(QFrame::HLine);
      vHSep->setLineWidth(1);
    }

		//vKeyPrompt

		this->vLayout->addWidget(this->vKeyPrompt.get(), 3);
		this->vKeyPrompt->setReadOnly(1);
		this->vKeyPrompt->setText(
			"Hit one of the keys:"
			"\n- 's' to add 1 step to the simulation progress;"
			"\n- 'a' to use antibiotics;"
			"\n- 'q' to quit;"
		);

    {
      auto vHSep = new QFrame(this);
      this->vLayout->addWidget(vHSep, 1);
      vHSep->setFrameShape(QFrame::HLine);
      vHSep->setLineWidth(1);
    }

		//vSimStateReport

		this->vLayout->addWidget(this->vSimStateReport.get(), 2);
		this->vSimStateReport->setReadOnly(1);

    {
      auto vHSep = new QFrame(this);
      this->vLayout->addWidget(vHSep, 1);
      vHSep->setFrameShape(QFrame::HLine);
      vHSep->setLineWidth(1);
    }

		//vSimBeingReport

		this->vLayout->addWidget(this->vSimBeingReport.get(), 8);
		for(auto vUnitIndex = 0; vUnitIndex < dUnitCount; vUnitIndex++) {
			auto vUnitOrder = vUnitIndex + 1;
			auto vUnitLabel = fmt::format("[{}]", vUnitOrder);
			this->vSimBeingReport->fAddWidget(new QLabel(vUnitLabel.c_str(), this));
		}

		//Font

		auto vFontFpath = "data/FiraMono-Bold.ttf";
		fLogErr("vFontFpath", "\"{}\";", vFontFpath);
		auto vFontIndex = QFontDatabase::addApplicationFont(vFontFpath);
		fLogErr("vFontIndex", "{};", vFontIndex);
		auto vFontIdentArray = QFontDatabase::applicationFontFamilies(vFontIndex);
		fLogErr("vFontIdentArray.length()", "{};", vFontIdentArray.length());
		auto vFontIdent = vFontIdentArray.at(0);
		fLogErr("vFontIdent", "\"{}\";", vFontIdent.toUtf8().constData());
		auto vFontEntry = QFont(vFontIdent, 16);
		vFontEntry.setBold(true);
		this->setFont(vFontEntry);

		//Window show

		this->setWindowTitle(dProjName);

		auto vRect = QGuiApplication::primaryScreen()->geometry();
		this->resize(vRect.width() / 2, vRect.height());

		this->show();
	}//ctor

	///actions

	void tWin::keyPressEvent(QKeyEvent *vQKeyEvent) {
		auto vApp = reinterpret_cast<tApp *>(QApplication::instance());
		vApp->fTryEventKeyPress(vQKeyEvent);
		QWidget::keyPressEvent(vQKeyEvent);
	}

	//}

	//{

	tSimBeingReport::tSimBeingReport(QWidget *vParentPtr)
		: QScrollArea(vParentPtr), vLayout{std::make_shared<QVBoxLayout>(this)} {
		this->setLayout(this->vLayout.get());
	}

	void tSimBeingReport::fAddWidget(QWidget *vWidgetPtr) {
		this->vLayout->addWidget(vWidgetPtr, 1);
	}

	//}

}

//application entry point

int main(int vArgC, char **vArgV) {
	fmt::println(stderr, "[main]=(");
	try {
		fLogErr("vArgC", "{};", vArgC);
		fLogErr("vArgV[0]", "\"{}\";", vArgV[0]);
		fLogErr("dProjName", "\"{}\";", dProjName);
		fLogErr("dWorkPath", "\"{}\";", std::filesystem::current_path().c_str());
		fLogErr("QT_VERSION_STR", "\"{}\";", QT_VERSION_STR);
		auto vApp = tApp(vArgC, vArgV);
		auto vErr = vApp.exec();
		fLogErr("error", "{};", vErr);
		if(vErr) {
			return vErr;
		}
	} catch(const std::exception &vError) {
		fLogErr("[exception]", "\"{}\";", vError.what());
		return 1;
	}
	fmt::println(stderr, ")=[main]");
	return 0;
}
