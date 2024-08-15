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

	public://actions

		std::optional<tSimBeing> fRunStep();

		bool fTryAntibio();

	private://actions

		bool fTryDeath();

		std::optional<tSimBeing> fTryRepro();

	private://vardefs

		//lifetime
		long			 vAliveStepCount;//how many steps since birth?
		const long vAliveStepLimit;//how many steps until death?
		bool			 vAliveValue;		 //are we alive?

		//reproduction
		long			 vReproStepCount;//how many steps since the last reproduction?
		const long vReproStepLimit;//how many steps until the next reproduction?
		const long vReproIndex;		 //the current generation number

	};//tSimBeing

}//simulation

namespace {//system

	/* application giga-class
	 * - creates and holds tWin instance
	 */
	class tWin;
	class tApp: public QApplication {
	public:

		tApp(int vArgC, char **vArgV);

		bool fTryEventKeyPress(QKeyEvent *vQKeyEvent);

	private:

		std::shared_ptr<tWin> vWin;

		int vSimStepIndex;

		QList<tSimBeing> vSimBeingArray;

	};//tApp

	/* window of application
	 * - creates and holds tSimBeingReport
	 */
	class tSimBeingReport;
	class tWin: public QWidget {
	public://codetor

		tWin(
			QWidget *vParentPtr = nullptr, Qt::WindowFlags vFlags = Qt::WindowFlags()
		);

	public://actions

		void keyPressEvent(QKeyEvent *vQKeyEvent) override;

	public://getters

		auto fGetKeyOutput() {
			return this->vKeyOutput;
		}

	private://vardefs

		std::shared_ptr<QVBoxLayout> vLayout;

		std::shared_ptr<QTextEdit> vKeyPrompt;
		std::shared_ptr<QTextEdit> vKeyOutput;

		std::shared_ptr<QTextEdit>			 vSimStateReport;
		std::shared_ptr<tSimBeingReport> vSimBeingReport;

	};//tWin

	/* window to monitor states of all organisms
	 */
	class tSimBeingReport: public QScrollArea {
	public:

		tSimBeingReport(QWidget *vParentPtr);

		void fAddWidget(QWidget *vWidgetPtr);

	private:

		std::shared_ptr<QVBoxLayout> vLayout;

	};//tSimBeingReport

}//system

//entry point
extern int main(int vArgC, char **vArgV);

#endif//dChalGameOfLife_exe_hpp
