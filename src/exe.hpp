#include "top.hpp"

#ifndef dChalGameOfLife_exe_hpp
#define dChalGameOfLife_exe_hpp

namespace {//simulation

	/* simulation being class
	 * - reproduces itself once in a constant amount of time
	 * - dies after a constant amount of time
	 * - can have a parent, inheriting it's timing settings
	 */
	class tSimBeing {
	public://consdef

		static constexpr auto vAntibioSurviveChance = 3;
		static constexpr auto vAntibioKillingChance = 10;
		static_assert(
			vAntibioSurviveChance < vAntibioKillingChance,
			"survival chance is included in the killing chance;"
			"an organism survives if a random value [1,AntibioKillingChance]"
			" is less than or equal to the AntibioSurviveChance;"
			"an organism dies if a random value [1,AntibioKillingChance]"
			" is more than the AntibioSurviveChance;"
		);

	public://codetor

		tSimBeing(std::optional<tSimBeing> vAncestor = {});

	public://getters

		inline auto fGetAliveStepCount() const {
			return this->vAliveStepCount;
		}
		inline auto fGetAliveStepLimit() const {
			return this->vAliveStepLimit;
		}

		inline auto fGetReproStepCount() const {
			return this->vReproStepCount;
		}
		inline auto fGetReproStepLimit() const {
			return this->vReproStepLimit;
		}
		inline auto fGetReproIndex() const {
			return this->vReproIndex;
		}

	public://vetters

		inline auto fVetAlive(bool vAliveValue = 1) const {
			return (this->vAliveValue == vAliveValue);
		}

	public slots:

		std::optional<tSimBeing> fRunStep();

		bool fTryAntibio();

	private://actions

		bool fTryDeath();

		std::optional<tSimBeing> fTryRepro();

	private://datadef

		//lifetime
		long			 vAliveStepCount;//how many steps since birth?
		const long vAliveStepLimit;//how many steps until death?
		bool			 vAliveValue;		 //are we alive?

		//reproduction
		long			 vReproStepCount;//how many steps since the last reproduction?
		const long vReproStepLimit;//how many steps until the next reproduction?
		const long vReproIndex;		 //the current generation number
	};

	class tSimThread: public QThread {
	public://codetor

		tSimThread();

	public://actions

		void run() override;
	};

}

namespace {//system

	/* application giga-class
	 * - creates and holds tAppWindow instance
	 */
	class tAppWindow;
	class tApp: public QApplication {
	public://codetor

		tApp(int vArgC, char **vArgV);

	public://actions

    void fRunSim(int vSimBeing1stCount);

		void fRunEventKeyPress(QKeyEvent *vQKeyEvent);

	public://setters

		void fAddSimStep();

	public://datadef

		std::shared_ptr<tAppWindow> vWindow;

		int vSimStepIndex;
    int vSimBeing1stCount;

		QVector<tSimBeing> vSimBeingArray;
	};

	/* application window:
	 * creates and holds tSimBeingReport;
	 */
	class t1stWindow;
	class tSimWindow;
	class tAppWindow: public QMainWindow {
	public://codetor

		tAppWindow(
			QWidget *vParent = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags()
		);

	public slots:

		void fSetSimWindow();

	public://datadef

		std::shared_ptr<t1stWindow> v1stWindow;
		std::shared_ptr<tSimWindow> vSimWindow;

		std::shared_ptr<QStackedWidget> vStack;
	};

	/* 1st window
	 * prompt users to input a number of simulation beings;
	 */
	class t1stWindow: public QWidget {
	public://codetor

		t1stWindow(tAppWindow *vAppWindow);

	public slots:

		void fRunSimConfig();

	public://consdef

		static constexpr auto vSimBeingMinCount = 1;
		static constexpr auto vSimBeingMaxCount = 20;

	public://datadef

		std::shared_ptr<QVBoxLayout> vLayout;

		std::shared_ptr<QTextEdit>	 vPrompt;
		std::shared_ptr<QPushButton> vButton;
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

		std::shared_ptr<QVBoxLayout> vLayout;

		std::shared_ptr<QTextEdit> vPrompt;
		std::shared_ptr<QTextEdit> vOutput;

		std::shared_ptr<tSimReport> vReport;

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

		std::shared_ptr<QVBoxLayout>			vLayout;
		std::shared_ptr<QTextEdit>				vStatus;
		std::shared_ptr<tSimReportScroll> vScroll;
	};

	/* simulation report scroll
	 */
	class tSimReportScroll: public QScrollArea {
	public://codetor

		tSimReportScroll(tSimReport *vSimReport);

	public://datadef

		std::shared_ptr<QVBoxLayout> vLayout;
	};

}

extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
