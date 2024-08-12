#include "gui.hpp"

auto vFontFpath = "data/FiraMono-Bold.ttf";
auto vStyleSheet = R"(
QWidget {
  background: black;
  alternate-background-color: dark-gray;
  accent-color: light-gray;
  color: cyan;
  placeholder-text-color: dark-cyan;
  selection-color: light-cyan;
}
)";

namespace nGui {
/* tApp */
tApp::tApp(int vArgC, char **vArgV)
	: QApplication(vArgC, vArgV), vWinRef{std::make_shared<tWin>()} {
	this->setStyleSheet(vStyleSheet);
}
/* tWin */
tWin::tWin(QWidget *vParentPtr, Qt::WindowFlags vFlags)
	: QWidget(vParentPtr, vFlags), vLayout{std::make_shared<QVBoxLayout>()} {
	this->setLayout(this->vLayout.get());

	this->vLayout->addWidget(new QLabel("Hello World", this), 1);
  
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
