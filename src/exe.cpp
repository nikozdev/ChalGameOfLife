#include "exe.hpp"

//{

void fAddGuiHorLine(auto vParent, auto &vLayout) {
	auto vHSep = new QFrame(vParent);
	vLayout->addWidget(vHSep, 1);
	vHSep->setFrameShape(QFrame::HLine);
	vHSep->setLineWidth(1);
}

//}

//{

///{

//codetor

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

//actions

std::optional<tSimBeing> tSimBeing::fRunStep() {
	if(this->fVetAlive(0)) {
		fLogErr("fRunStep", "Running step for a dead organism!");
	}
	std::optional<tSimBeing> vChild;
	if(this->vReproStepCount < this->vReproStepLimit) {
		this->vReproStepCount += 1;
	} else {
		//vChild.emplace(this->fTryRepro());
	}
	if(this->vAliveStepCount < this->vAliveStepLimit) {
		this->vAliveStepCount += 1;
	} else {
		this->fTryDeath();
	}
	//return vChild;
  return {};
}

bool tSimBeing::fTryAntibio() {
	if(this->fVetAlive(0)) {
		fLogErr("fTryAntibio", "Trying antibio on a dead organism!");
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
		fLogErr("fTryDeath", "Trying death on a dead organism!");
	}
	this->vAliveValue = 0;
	return 1;
}

std::optional<tSimBeing> tSimBeing::fTryRepro() {
	if(this->fVetAlive(0)) {
		fLogErr("fTryRepro", "Trying repro on a dead organism!");
	}
	return tSimBeing(*this);
}

///}

//}

//{

///{

//codetor

tApp::tApp(int vArgC, char **vArgV)
	: QApplication(vArgC, vArgV), vWindow{new tAppWindow()}, vSimStepIndex(0) {
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

//actions

void tApp::fRunSim(int vSimBeing1stCount) {
	if(this->vSimBeing1stCount) {
		fLogErr("fRunSim Error", "Cannot set simulation twice!");
	}
	this->vSimBeing1stCount = vSimBeing1stCount;
	this->vSimBeingArray		= QVector<tSimBeing>(vSimBeing1stCount);
	emit this->sRunSimCall();
}

void tApp::fRunEventKeyPress(QKeyEvent *vQKeyEvent) {
	auto vQKeyBytes = vQKeyEvent->text().toLower().toUtf8().constData();
	fLogErr("vQKeyBytes", "{};", vQKeyBytes);
	auto vQKeyValue = vQKeyBytes[0];
	switch(vQKeyValue) {
	case 'a': {
		auto vTotalCount = this->vSimBeingArray.size();
		auto vAliveCount = 0;
		for(auto vIndex = 0; vIndex < vTotalCount; vIndex++) {
			auto &vEntry = this->vSimBeingArray[vIndex];
			vEntry.fTryAntibio();
			vAliveCount += vEntry.fVetAlive(1);
		}
		this->vWindow->vSimWindow->vOutput->setText(
			fmt::format(
				"[{}]: "
				"Using antibiotics...\n"
				"({}/{} left alive);",
				vQKeyValue,
				vAliveCount,
				vTotalCount
			)
				.c_str()
		);
	} break;
	case 's': {
		auto vSimStepWas = this->vSimStepIndex;
		this->fAddSimStep();
		auto vSimStepNow = this->vSimStepIndex;
		this->vWindow->vSimWindow->vOutput->setText(
			fmt::format(
				"[{}]: "
				"Taking 1 more step of simulation\n"
				"(from {} to {});",
				vQKeyValue,
				vSimStepWas,
				vSimStepNow
			)
				.c_str()
		);
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
		this->vWindow->vSimWindow->vOutput
			->setText(fmt::format("[{}]: Nothing to do;", vQKeyValue).c_str());
	} break;
	}
}

//setters

void tApp::fAddSimStep() {
	this->vSimStepIndex++;
}

///}

///{

//codetor

tAppWindow::tAppWindow(QWidget *vParentPtr, Qt::WindowFlags vFlags)
	: QMainWindow(vParentPtr, vFlags)
	, v1stWindow{new t1stWindow(this)}
	, vSimWindow{new tSimWindow(this)}
	, vStack{new QStackedWidget(this)} {
	fLogErr("AppWindow", "ctor");
	{//Stack

		this->setCentralWidget(this->vStack.get());

		this->vStack->addWidget(this->v1stWindow.get());
		this->vStack->addWidget(this->vSimWindow.get());

		this->vStack->setCurrentIndex(0);
			connect(
				static_cast<tApp *>(QApplication::instance()),
				&tApp::sRunSimCall,
				this,
				&tAppWindow::sRunSimSlot
			);
	}
	{//Fonts

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
	}
	{//Window show

		this->setWindowTitle(dProjName);

		auto vRect = QGuiApplication::primaryScreen()->geometry();
		this->resize(vRect.width() / 2, vRect.height());

		this->show();
	}
}

//slots

void tAppWindow::sRunSimSlot() {
	this->vStack->setCurrentIndex(1);
}

///}

///{

//codetor

t1stWindow::t1stWindow(tAppWindow *vAppWindow)
	: QWidget(vAppWindow)
	, vLayout{new QVBoxLayout(this)}
	, vPrompt{new QTextEdit("start prompt", this)}
	, vButton{new QPushButton("start", this)} {
	fLogErr("1stWindow", "ctor");
	this->setLayout(this->vLayout.get());

	this->vLayout->addWidget(this->vPrompt.get(), 4);
	this->vPrompt->setReadOnly(1);
	this->vPrompt->setText(
		"Welcome to the micro-organism simulation!\n"
		"Before we run the simulation,"
		" we want you to configure this."
		"\n"
		"The configuration is gonna start"
		" as soon as you hit the button below."
	);

	this->vLayout->addWidget(this->vButton.get(), 1);
	this->vButton->setText("Config");
	connect(
		this->vButton.get(), &QPushButton::clicked, this, &t1stWindow::fRunSimConfig
	);
}

//slots

void t1stWindow::fRunSimConfig() {
	bool vInputFlag;
	auto vSimBeing1stCount = QInputDialog::getInt(
		this,
		"Starting Simulation",
		fmt::format(
			"Enter the number of micro-organisms from {} to {}.",
			this->vSimBeingMinCount,
			this->vSimBeingMaxCount
		)
			.c_str(),
		0,
		this->vSimBeingMinCount,
		this->vSimBeingMaxCount,
		1,
		&vInputFlag
	);
	if(vInputFlag) {
		auto vApp = static_cast<tApp *>(QApplication::instance());
		vApp->fRunSim(vSimBeing1stCount);
	}
}

///}

///{

tSimWindow::tSimWindow(tAppWindow *vAppWindow)
	: QWidget(vAppWindow)
	, vLayout{new QVBoxLayout(this)}
	, vPrompt{new QTextEdit("key prompt", this)}
	, vOutput{new QTextEdit("key output", this)}
	, vReport{new tSimReport(this)} {
	fLogErr("SimWindow", "ctor");
	this->setLayout(this->vLayout.get());

	this->vLayout->addWidget(this->vPrompt.get(), 3);
	this->vPrompt->setReadOnly(1);
	this->vPrompt->setText(
		"Hit one of the keys:"
		"\n- 's' to add 1 step to the simulation progress;"
		"\n- 'a' to use antibiotics;"
		"\n- 'q' to quit;"
	);

	fAddGuiHorLine(this, this->vLayout);

	this->vLayout->addWidget(this->vOutput.get(), 2);
	this->vOutput->setReadOnly(1);

	fAddGuiHorLine(this, this->vLayout);

	this->vLayout->addWidget(this->vReport.get(), 8);
}

void tSimWindow::keyPressEvent(QKeyEvent *vQKeyEvent) {
	auto vApp = static_cast<tApp *>(QApplication::instance());
	vApp->fRunEventKeyPress(vQKeyEvent);
	QWidget::keyPressEvent(vQKeyEvent);
}

///}

///{

//codetor

tSimReport::tSimReport(tSimWindow *vSimWindow)
	: QWidget(vSimWindow)
	, vLayout{new QVBoxLayout(this)}
	, vStatus{new QTextEdit(this)}
	, vScroll{new tSimReportScroll(this)} {
	fLogErr("SimReport", "ctor");
	this->vLayout->addWidget(this->vStatus.get(), 1);
	this->vStatus->setReadOnly(1);

	fAddGuiHorLine(this, this->vLayout);
	this->vLayout->addWidget(this->vScroll.get(), 4);
}

///}

///{

//codetor

tSimReportScroll::tSimReportScroll(tSimReport *vSimReport)
	: QScrollArea(vSimReport), vLayout{new QVBoxLayout(this)} {
	fLogErr("SimReportScroll", "ctor");
	this->setLayout(this->vLayout.get());
}

//slots

void tSimReportScroll::sRunSimSlot() {
}

///}

//}

int main(int vArgC, char **vArgV) {
	fmt::println(stderr, "[main]=(");
	try {
		fLogErr("vArgC", "{};", vArgC);
		fLogErr("vArgV[0]", "\"{}\";", vArgV[0]);
		fLogErr("dProjName", "\"{}\";", dProjName);
		fLogErr("dWorkPath", "\"{}\";", std::filesystem::current_path().c_str());
		fLogErr("QT_VERSION_STR", "\"{}\";", QT_VERSION_STR);
		{
			auto vApp = tApp(vArgC, vArgV);
			auto vErr = vApp.exec();
			fLogErr("error", "{};", vErr);
			if(vErr) {
				return vErr;
			}
		}
	} catch(const std::exception &vError) {
		fLogErr("[exception]", "\"{}\";", vError.what());
		return 1;
	}
	fmt::println(stderr, ")=[main]");
	return 0;
}
