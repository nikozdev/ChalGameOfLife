#include "top.hpp"

#ifndef dChalGameOfLife_exe_hpp
#define dChalGameOfLife_exe_hpp

//{

/* simulation being class
 */
struct tSimBeing {
public://consdef

	//limits in seconds
	inline static auto vReproTimerLimitRange = std::uniform_int_distribution<
		unsigned>(2, 4);
	inline static auto vAliveTimerLimitRange = std::uniform_int_distribution<
		unsigned>(4, 8);

public://codetor

	tSimBeing(tSimBeing *vAncestor = nullptr)
		: vAliveTimerSince{std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::steady_clock::now().time_since_epoch()
			)}
		, vAliveTimerLimit{vAncestor ? vAncestor->vAliveTimerLimit : std::chrono::seconds(vAliveTimerLimitRange(vRandomGen))}
		, vReproTimerSince{vAliveTimerSince}
		, vReproTimerLimit{vAncestor ? vAncestor->vReproTimerLimit : std::chrono::seconds(vReproTimerLimitRange(vRandomGen))}
		, vReproIndex{vAncestor ? vAncestor->vReproIndex + 1 : 0} {
		fLogErr(
			"tSimBeing::tSimBeing",
			"{}; {}; {}; {}; {}; {};",
			(void *)vAncestor,
			vAliveTimerSince,
			vAliveTimerLimit,
			vReproTimerSince,
			vReproTimerLimit,
			vReproIndex
		);
	}

public://datadef

	const std::chrono::seconds vAliveTimerSince;//how long since birth?
	const std::chrono::seconds vAliveTimerLimit;//how long until death?

	const std::chrono::seconds vReproTimerSince;//how long since birth?
	const std::chrono::seconds vReproTimerLimit;//how long until repro?

	const long vReproIndex;//the current generation number
};

class tSimThread: public QThread {
	Q_OBJECT

public://codetor

	tSimThread(size_t vIndex);

public://actions

	void run() override;

	void fRunStep();

public slots:

	void sTakeAntibioSlot();

signals:

	void sGiveUpdateCall(const size_t vIndex, QVector<tSimBeing> vSimBeingArray);

public://datadef

	size_t vIndex;
	QMutex vMutex;

	QVector<tSimBeing> vBeingArray;
};

//}

//{system

/* application giga-class
 * - creates and holds tAppWindow instance
 */
class tAppWindow;
class tApp: public QApplication {
	Q_OBJECT

public://codetor

	tApp(int vArgC, char **vArgV);

public://actions

	void fRunSim(unsigned vSimThreadCount);

	void fRunEventKeyPress(QKeyEvent *vQKeyEvent);

public slots:

	void sAboutToQuitSlot();

signals:

	void sRunSimCall();

	void sTakeAntibioCall();

public://datadef

	std::unique_ptr<tAppWindow> vWindow;

	QVector<std::shared_ptr<tSimThread>> vSimThreadArray;

	QVector<tSimBeing> vSimBeingArray;
};

/* application window:
 * creates and holds tSimBeingReport;
 */
class t1stWindow;
class tSimWindow;
class tAppWindow: public QMainWindow {
	Q_OBJECT

public://codetor

	tAppWindow(
		QWidget *vParent = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags()
	);

public slots:

	void sRunSimSlot();

public://datadef

	std::unique_ptr<t1stWindow> v1stWindow;
	std::unique_ptr<tSimWindow> vSimWindow;

	std::unique_ptr<QStackedWidget> vStack;
};

/* 1st window
 * prompt users to input a number of simulation beings;
 */
class t1stWindow: public QWidget {
	Q_OBJECT

public://codetor

	t1stWindow(tAppWindow *vAppWindow);

public slots:

	void fRunSimConfig();

public://consdef

	static constexpr auto vSimThreadMinCount = 1;
	static constexpr auto vSimThreadMaxCount = 20;

public://datadef

	std::unique_ptr<QVBoxLayout> vLayout;

	std::unique_ptr<QTextEdit>	 vPrompt;
	std::unique_ptr<QPushButton> vButton;
};

/* simulation window
 */
class tSimReport;
class tSimWindow: public QWidget {
public://codetor

	tSimWindow(tAppWindow *vAppWindow);

public://actions

	void keyPressEvent(QKeyEvent *vQKeyEvent) override;

public://datadef

	std::unique_ptr<QVBoxLayout> vLayout;

	std::unique_ptr<QTextEdit> vPrompt;
	std::unique_ptr<QTextEdit> vOutput;

	std::unique_ptr<tSimReport> vReport;

	/* window to monitor states of all organisms
	 */
};

/* simulation report
 * display information about the simulation
 * hold scrolling area for simulation beings
 */
class tSimReportScroll;
class tSimReport: public QWidget {
public://codetor

	tSimReport(tSimWindow *vSimWindow);

public://datadef

	std::unique_ptr<QVBoxLayout>			vLayout;
	std::unique_ptr<QTextEdit>				vStatus;
	std::unique_ptr<tSimReportScroll> vScroll;
};

/* simulation report scroll
 */
class tSimReportScrollWidget;
class tSimReportScroll: public QScrollArea {
public://codetor

	tSimReportScroll(tSimReport *vSimReport);

public://datadef

	std::unique_ptr<tSimReportScrollWidget> vWidget;
};

class tSimReportScrollWidget: public QWidget {
	Q_OBJECT

public://codetor

	tSimReportScrollWidget(tSimReportScroll *vSimReportScroll);
	~tSimReportScrollWidget();

public://actions

	void fClean();

public slots:

	void sRunSimSlot();

	void sGiveUpdateSlot(
		const size_t vSimThreadIndex, QVector<tSimBeing> vSimBeingArray
	);

public://datadef

	std::unique_ptr<QVBoxLayout> vLayout;
};

//}

extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
