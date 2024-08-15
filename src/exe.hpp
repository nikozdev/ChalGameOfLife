#include "top.hpp"

#ifndef dChalGameOfLife_exe_hpp
#define dChalGameOfLife_exe_hpp

//{

/* simulation being class
 */
class tSimBeing: public QObject {
	Q_OBJECT

public://codetor

	tSimBeing(tSimBeing *vAncestor = nullptr);
	~tSimBeing();

public://actions

	void fRun();

public slots:

	void fTryRepro();
	void fTryDeath();

signals:

	void sTryReproCall(tSimBeing *vSimBeing);
	void sTryDeathCall(tSimBeing *vSimBeing);

public://consdef

	static constexpr auto vAntibioSurviveChance = 3;
	static constexpr auto vAntibioKillingChance = 10;

	inline static auto vReproTimerLimitRange = std::uniform_int_distribution<
		int>(10'000, 30'000);
	inline static auto vAliveTimerLimitRange = std::uniform_int_distribution<
		int>(10'000, 30'000);

	static_assert(
		vAntibioSurviveChance < vAntibioKillingChance,
		"survival chance is included in the killing chance;"
		"an organism survives if a random value [1,AntibioKillingChance]"
		" is less than or equal to the AntibioSurviveChance;"
		"an organism dies if a random value [1,AntibioKillingChance]"
		" is more than the AntibioSurviveChance;"
	);

public://datadef

	QTimer													vAliveTimer;
	const std::chrono::milliseconds vAliveTimerPoint;//how long since birth?
	const std::chrono::milliseconds vAliveTimerLimit;//how long until death?

	QTimer													vReproTimer;
	const std::chrono::milliseconds vReproTimerPoint;//how long since birth?
	const std::chrono::milliseconds vReproTimerLimit;//how long until repro?
	const long											vReproIndex;//the current generation number
};

class tSimThread: public QThread {
	Q_OBJECT

public://codetor

	tSimThread();

public://actions

	void run() override;

public://getters

  const QVector<tSimBeing> &fStartRead() {
		return this->vBeingArrayWas;
	}
	const QVector<tSimBeing> &fGiveBeingArray() {
		return this->vBeingArrayWas;
	}

public://datadef

	QVector<tSimBeing> vBeingArrayWas;
	QVector<tSimBeing> vBeingArrayNow;

	QMutex vMutex;
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

	void fRunSim(unsigned vSimBeing1stCount);

	void fRunEventKeyPress(QKeyEvent *vQKeyEvent);

signals:

	void sRunSimCall();

public://datadef

	std::unique_ptr<tAppWindow> vWindow;

	unsigned						vSimThread1stCount;
	QVector<tSimThread> vSimThreadArray;
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
class tSimReportScroll: public QScrollArea {
	Q_OBJECT

public://codetor

	tSimReportScroll(tSimReport *vSimReport);

public slots:

	void sRunSimSlot();

public://datadef

	std::unique_ptr<QVBoxLayout> vLayout;
};

//}

extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
