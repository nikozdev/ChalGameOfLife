#include "gui.hpp"

static auto vFontFpath		= "data/FiraMono-Bold.ttf";
static auto vStyleSheet		= R"(
QWidget {
  background: black;
  alternate-background-color: dark-gray;
  accent-color: light-gray;
  color: cyan;
  placeholder-text-color: dark-cyan;
  selection-color: light-cyan;
}
)";
static auto vKeyHintsText = "<space> to pause; k to kill; q to quit;";

namespace nGui {
/* tApp */
tApp::tApp(int vArgC, char **vArgV)
	: QApplication(vArgC, vArgV), vWin{std::make_shared<tWin>()} {
	this->setStyleSheet(vStyleSheet);
  this->vWin->fSetKeyCallback('q', QApplication::quit);
}
/* tWin */
tWin::tWin(QWidget *vParentPtr, Qt::WindowFlags vFlags)
	: QWidget(vParentPtr, vFlags)
	, vLayout{std::make_shared<QVBoxLayout>(this)}
	, vUnitMonitor{std::make_shared<tUnitMonitor>(this)}
	, vInfoLabel{std::make_shared<QLabel>("some random information", this)}
	, vKeyHints{std::make_shared<QLabel>(vKeyHintsText, this)} {
	this->setWindowTitle(dProjName);

	this->setLayout(this->vLayout.get());

	for(auto vUnitIndex = 0; vUnitIndex < dUnitCount; vUnitIndex++) {
		auto vUnitOrder = vUnitIndex + 1;
		auto vUnitLabel = fmt::format("[{}]", vUnitOrder);
		this->vUnitMonitor->fAddWidget(new QLabel(vUnitLabel.c_str(), this));
	}
	this->vLayout->addWidget(this->vUnitMonitor.get(), 8);

	auto vHSep0 = new QFrame(this);
	vHSep0->setFrameShape(QFrame::HLine);
	vHSep0->setLineWidth(1);
	this->vLayout->addWidget(vHSep0, 1);

	this->vLayout->addWidget(this->vInfoLabel.get(), 1);

	auto vHSep1 = new QFrame(this);
	vHSep1->setFrameShape(QFrame::HLine);
	vHSep1->setLineWidth(1);
	this->vLayout->addWidget(vHSep1, 1);

	this->vLayout->addWidget(this->vKeyHints.get(), 1);

	fmt::println("[vFontFpath]={};", vFontFpath);
	auto vFontIndex = QFontDatabase::addApplicationFont(vFontFpath);
	fmt::println("[vFontIndex]={};", vFontIndex);
	auto vFontIdentArray = QFontDatabase::applicationFontFamilies(vFontIndex);
	fmt::println("[vFontIdentArray][length]={};", vFontIdentArray.length());
	auto vFontIdent = vFontIdentArray.at(0);
	fmt::println("[vFontIdent]={};", vFontIdent.toUtf8().constData());
	auto vFontEntry = QFont(vFontIdent, 16);
	vFontEntry.setBold(true);
	this->setFont(vFontEntry);

	auto vScreen		 = QGuiApplication::primaryScreen();
	auto vScreenRect = vScreen->geometry();
	this->resize(vScreenRect.width() / 2, vScreenRect.height());

	this->show();
}
void tWin::keyPressEvent(QKeyEvent *vQKeyEvent) {
  auto vQKeyBytes = vQKeyEvent->text().toUtf8().constData();
  fmt::println(stderr, "[vQKeyBytes]={}", vQKeyBytes);
  this->fTryKeyCallback(vQKeyBytes[0]);
  QWidget::keyPressEvent(vQKeyEvent);
}
/* tUnitMonitor */
tUnitMonitor::tUnitMonitor(QWidget *vParentPtr)
	: QScrollArea(vParentPtr), vLayout{std::make_shared<QVBoxLayout>(this)} {
	this->setLayout(this->vLayout.get());
}
void tUnitMonitor::fAddWidget(QWidget *vWidgetPtr) {
	this->vLayout->addWidget(vWidgetPtr, 1);
}
}//namespace nGui
namespace nGui {
int fRun(int vArgC, char **vArgV) {
	return 0;
}
int fTry(int vArgC, char **vArgV) {
	fmt::println(stderr, "[gui]=(");
	fmt::println(stderr, R"([QT_VERSION_STR]="{}";)", QT_VERSION_STR);
	auto vApp					 = tApp(vArgC, vArgV);
	auto vAppErrorCode = vApp.exec();
	fmt::println(stderr, R"([vAppErrorCode]="{}";)", vAppErrorCode);
	fmt::println(stderr, ")=[gui]");
	return vAppErrorCode;
}
}//namespace nGui
