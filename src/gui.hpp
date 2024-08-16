#include "top.hpp"

#ifndef dChalGameOfLife_gui_hpp
#define dChalGameOfLife_gui_hpp

namespace nSim {
	class tBeing;
}//namespace nSim

namespace nGui {

	/* application window:
	 * switches between 2 windows using stacked widget:
	 * - creates and holds t1stWindow;
	 * - creates and holds tSimWindow;
	 */
	class t1stWindow;
	class tSimWindow;
	class tAppWindow: public QMainWindow {
		Q_OBJECT

	public://codetor

		tAppWindow(
			QWidget *vParent = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags()
		);

		~tAppWindow();

	public://varsdef

		t1stWindow *v1stWindow;
		tSimWindow *vSimWindow;

		QStackedWidget *vStack;
	};

	/* 1st window:
	 * provides a starting dialog;
	 * prompt users to input a number of simulation beings;
	 */
	class t1stWindow: public QWidget {
		Q_OBJECT

	public://codetor

		t1stWindow(tAppWindow *vAppWindow);

		~t1stWindow();

	public://consdef

		static constexpr auto vSimBeingMinCount = 1;
		static constexpr auto vSimBeingMaxCount = 20;

	public://varsdef

		QVBoxLayout *vLayout;

		QTextEdit		*vPrompt;
		QPushButton *vButton;
	};

	/* simulation window:
	 * holds the "report" widget;
	 * handles key presses;
	 * outputs information about each key press;
	 * prompts a user to press a key;
	 */
	class tSimReport;
	class tSimWindow: public QWidget {
	public://codetor

		tSimWindow(tAppWindow *vAppWindow);

		~tSimWindow();

	public://actions

		void keyPressEvent(QKeyEvent *vQKeyEvent) override;

	public://varsdef

		QVBoxLayout *vLayout;

		QTextEdit *vPrompt;
		QTextEdit *vOutput;

		tSimReport *vReport;
	};

	/* simulation report:
	 * display information about the simulation;
	 * hold scrolling area widget for simulation beings;
	 */
	class tSimReportScroll;
	class tSimReport: public QWidget {
	public://codetor

		tSimReport(tSimWindow *vSimWindow);

		~tSimReport();

	public://varsdef

		QVBoxLayout			 *vLayout;
		QTextEdit				 *vStatus;
		tSimReportScroll *vScroll;
	};

	/* simulation report scroll widget:
	 * writes information about every single simulation being;
	 * keeps track of all simulation beings via their signals;
	 */
	class tSimReportScrollWidget;
	class tSimReportScroll: public QScrollArea {
	public://codetor

		tSimReportScroll(tSimReport *vSimReport);

	public://varsdef

		tSimReportScrollWidget* vWidget;
	};

	class tSimReportScrollWidget: public QWidget {
		Q_OBJECT

	public://codetor

		tSimReportScrollWidget(tSimReportScroll *vSimReportScroll);
		~tSimReportScrollWidget();

	public://actions

		void fClean();

	public://varsdef

		QVBoxLayout* vLayout;
	};

}//namespace nGui

#endif//dChalGameOfLife_gui_hpp
