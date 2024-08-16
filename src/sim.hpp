#include "top.hpp"

#ifndef dChalGameOfLife_sim_hpp
#define dChalGameOfLife_sim_hpp

namespace nSim {

	/* simulation being:
	 * lives in a separate thread;
	 * after a random predefined amount of time it dies;
	 * once in a random predefined period of time it reproduces itself;
	 * can communicate with other threads using signals and slots;
	 */
	class tBeing: public QThread {
		Q_OBJECT

	public://typedef

		using tTimerPoint = std::chrono::milliseconds;
		using tTimerClock = std::chrono::steady_clock;

	public://codetor

		tBeing(tBeing *vAncestor = nullptr);

		~tBeing();

	public://actions

		void run() override;

	public slots:

		void sTryAntibioSlot();

	signals:

		void sAliveCall();//on each loop iteration;
		void sDeathCall();//after the loop;
		void sBirthCall();//periodically during the loop;

	public://consdef

		inline static constexpr auto vAntibioAliveChance = 2;
		inline static constexpr auto vAntibioDeathChance = 3;

		inline static constexpr auto vReproTimerLimitMin = 2;
		inline static constexpr auto vReproTimerLimitMax = 4;

		inline static constexpr auto vAliveTimerLimitMin = 4;
		inline static constexpr auto vAliveTimerLimitMax = 8;

	public://varsdef

		const tTimerPoint vAliveTimerSince;//how long since birth?
		const tTimerPoint vAliveTimerLimit;//how long until death?

		const tTimerPoint vReproTimerSince;//how long since birth?
		const tTimerPoint vReproTimerLimit;//how long until repro?

		const unsigned vReproIndex;//the current generation number

		QMutex vMutex;
	};

}//namespace nSim

#endif//dChalGameOfLife_sim_hpp
