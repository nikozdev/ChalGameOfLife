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

	std::shared_ptr<tWin> vWinRef;
};
class tWin: public QWidget {
public:

	tWin(QWidget *vParentPtr = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags());

private:

	std::shared_ptr<QVBoxLayout>	vLayout;
	std::shared_ptr<tUnitMonitor> vUnitMonitor;
};
class tUnitMonitor: public QScrollArea {
public:

	tUnitMonitor(QWidget *vParentPtr);

  void fAddWidget(QWidget* vWidgetPtr);

private:

	std::shared_ptr<QVBoxLayout> vLayout;
};
}//namespace nGui
namespace nGui {
int fRun(int vArgC, char **vArgV);
int fTry(int vArgC, char **vArgV);
}//namespace nGui

#endif//dChalGameOfLife_gui_hpp
