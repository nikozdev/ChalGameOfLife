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

tSimThread::tSimThread(size_t vIndex)
	: QThread(nullptr), vIndex(vIndex), vMutex(), vBeingArray{} {
}

//actions

void tSimThread::run() {
	this->vBeingArray.push_back(tSimBeing(nullptr));
	while(this->vBeingArray.size()) {
		this->msleep(1'000);
		this->fRunStep();
	}
}

void tSimThread::fRunStep() {
	QMutexLocker vMutexLocker(&this->vMutex);

	auto vBeingCount = this->vBeingArray.size();

	auto vChildArray = QVector<tSimBeing>();
	vChildArray.reserve(vBeingCount);

	auto vAliveArray = QVector<tSimBeing>();
	vAliveArray.reserve(vBeingCount);

	auto vTimerPoint = std::chrono::duration_cast<
		std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch());

	for(auto &vBeingEntry: this->vBeingArray) {
		auto vBirthTimerPoint
			= vBeingEntry.vReproTimerSince + vBeingEntry.vReproTimerLimit;
		if(vTimerPoint >= vBirthTimerPoint) {
			vChildArray.push_back(tSimBeing(&vBeingEntry));
			vBeingEntry.vReproTimerSince = vTimerPoint;
		}
		auto vDeathTimerPoint
			= vBeingEntry.vAliveTimerSince + vBeingEntry.vAliveTimerLimit;
		if(vTimerPoint <= vDeathTimerPoint) {
			vAliveArray.push_back(vBeingEntry);
		}
	}
	if(this->vBeingArray.size() != vAliveArray.size() || vChildArray.size()) {
		this->vBeingArray.clear();
		this->vBeingArray.reserve(vAliveArray.size() + vChildArray.size());

		this->vBeingArray.append(std::move(vAliveArray));
		this->vBeingArray.append(std::move(vChildArray));
	}
	emit this->sGiveUpdateCall(this->vIndex, this->vBeingArray);
}

//slots

void tSimThread::sTakeAntibioSlot() {
	QMutexLocker vMutexLocker(&this->vMutex);

	auto vBeingCount = vBeingArray.size();
	auto vAliveRange = std::uniform_int_distribution<unsigned>(0, vBeingCount);
	auto vAliveCount = vAliveRange(vRandomGen);

	if(this->vBeingArray.size() != vAliveCount) {
		auto vBeingArray = this->vBeingArray;
		this->vBeingArray.resize(vAliveCount);
		emit this->sGiveUpdateCall(this->vIndex, this->vBeingArray);
	}
}

///}

//}

//{

///{

//codetor

tApp::tApp(int vArgC, char **vArgV)
	: QApplication(vArgC, vArgV), vWindow{new tAppWindow()}, vSimThreadArray{} {
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

void tApp::fRunSim(unsigned vSimThreadCount) {
	fLogErr("fRunSim", "is being done;");
	if(this->vSimThreadArray.size()) {
		fLogErr("fRunSim Error", "Cannot set simulation twice!");
		return;
	}
	this->vSimThreadArray.reserve(vSimThreadCount);
	for(size_t vIndex = 0; vIndex < vSimThreadCount; vIndex++) {
		auto vSimThread = std::make_shared<tSimThread>(vIndex);
		this->vSimThreadArray.push_back(vSimThread);
		connect(
			this, &tApp::sTakeAntibioCall, vSimThread.get(), &tSimThread::sTakeAntibioSlot
		);
		vSimThread->start();
	}
	connect(this, &tApp::aboutToQuit, this, &tApp::sAboutToQuitSlot);
	fLogErr("fRunSim", "has been done;");
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
		emit this->sTakeAntibioCall();
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

//slots

void tApp::sAboutToQuitSlot() {
	for(auto &vSimThread: this->vSimThreadArray) {
		vSimThread->quit();
		vSimThread->wait();
	}
	this->vSimThreadArray.clear();
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
	auto vSimThreadCount = QInputDialog::getInt(
		this,
		"Starting Simulation",
		fmt::format(
			"Enter the number of micro-organisms from {} to {}.",
			this->vSimThreadMinCount,
			this->vSimThreadMaxCount
		)
			.c_str(),
		0,
		this->vSimThreadMinCount,
		this->vSimThreadMaxCount,
		1,
		&vInputFlag
	);
	if(vInputFlag) {
		auto vApp = static_cast<tApp *>(QApplication::instance());
		vApp->fRunSim(vSimThreadCount);
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
	: QScrollArea(vSimReport), vWidget{new tSimReportScrollWidget(this)} {
	fLogErr("SimReportScroll", "ctor");
	this->setWidget(this->vWidget.get());
}

///}

///{

//codetor

tSimReportScrollWidget::tSimReportScrollWidget(tSimReportScroll *vSimReportScroll
)
	: QWidget(vSimReportScroll), vLayout{new QVBoxLayout(this)} {
	fLogErr("SimReportScrollWidget", "ctor");
	this->setLayout(this->vLayout.get());
	connect(
		static_cast<tApp *>(QApplication::instance()),
		&tApp::sRunSimCall,
		this,
		&tSimReportScrollWidget::sRunSimSlot
	);
}
tSimReportScrollWidget::~tSimReportScrollWidget() {
	this->fClean();
}

//actions

void tSimReportScrollWidget::fClean() {
	while(auto vLayoutItem = this->vLayout->takeAt(0)) {
		delete vLayoutItem->widget();
		delete vLayoutItem;
	}
}

//slots

void tSimReportScrollWidget::sRunSimSlot() {
	auto vApp = static_cast<tApp *>(QObject::sender());

	const auto &vSimThreadArray = vApp->vSimThreadArray;
	const auto	vSimThreadCount = vSimThreadArray.size();
	fLogErr("RunSimSlot", "{}", vSimThreadCount);
	for(auto vSimThreadIndex = 0; vSimThreadIndex < vSimThreadCount;
			vSimThreadIndex++) {
		auto &vSimThreadEntry = vSimThreadArray[vSimThreadIndex];
		connect(
			vSimThreadEntry.get(),
			&tSimThread::sGiveUpdateCall,
			this,
			&tSimReportScrollWidget::sGiveUpdateSlot
		);
		auto vSimThreadOrder = vSimThreadIndex + 1;
		auto vSimThreadTitle = fmt::
			format("Thread {}/{}", vSimThreadOrder, vSimThreadCount);
		this->vLayout->addWidget(new QGroupBox(vSimThreadTitle.c_str(), this), 1);
	}

	this->show();
}

//slots

void tSimReportScrollWidget::sGiveUpdateSlot(
	const size_t vSimThreadIndex, QVector<tSimBeing> vSimBeingArray
) {
  fLogErr("sGiveUpdateCall", "[{}] {} beings;", vSimThreadIndex, vSimBeingArray.size());
	this->fClean();
	for(auto vSimBeingIndex = 0; vSimBeingIndex < vSimBeingArray.size();
			vSimBeingIndex++) {
		this->vLayout
			->addWidget(new QLabel(fmt::format("{}", vSimBeingIndex).c_str(), this));
	}
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
