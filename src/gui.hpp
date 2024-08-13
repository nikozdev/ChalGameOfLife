#include "top.hpp"

#ifndef dChalGameOfLife_gui_hpp
#define dChalGameOfLife_gui_hpp

namespace nGui {
class tApp;
class tWin;
class tUnitMonitor;
}//namespace nGui
namespace nGui {
class tApp: public QApplication {
public:

	tApp(int vArgC, char **vArgV);

private:

	std::shared_ptr<tWin> vWin;
};
class tWin: public QWidget {
public:

	tWin(QWidget *vParentPtr = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags());

	void keyPressEvent(QKeyEvent *vQKeyEvent) override;

	void fSetKeyCallback(char vKeySym, std::function<void()> vKeyCallback) {
		this->vKeyCallbackTable[vKeySym] = vKeyCallback;
	}
	void fTryKeyCallback(char vKeySym) {
		if(this->vKeyCallbackTable.find(vKeySym) != this->vKeyCallbackTable.end()) {
			this->vKeyCallbackTable[vKeySym]();
		}
	}

private:

	std::shared_ptr<QVBoxLayout>	vLayout;
	std::shared_ptr<tUnitMonitor> vUnitMonitor;
	std::shared_ptr<QLabel>				vInfoLabel;
	std::shared_ptr<QLabel>				vKeyHints;

	std::unordered_map<char, std::function<void()>> vKeyCallbackTable;
};
class tUnitMonitor: public QScrollArea {
public:

	tUnitMonitor(QWidget *vParentPtr);

	void fAddWidget(QWidget *vWidgetPtr);

private:

	std::shared_ptr<QVBoxLayout> vLayout;
};
}//namespace nGui
namespace nGui {
bool fRun(int vArgC, char **vArgV);
bool fTry(int vArgC, char **vArgV);
}//namespace nGui

#endif//dChalGameOfLife_gui_hpp
