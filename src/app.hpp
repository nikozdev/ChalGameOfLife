#include "top.hpp"

#ifndef dChalGameOfLife_app_hpp
#define dChalGameOfLife_app_hpp

namespace nSim {
	class tBeing;
}//namespace nSim

namespace nGui {
	class tAppWindow;
}//namespace nGui

namespace nApp {

	/* application giga-class:
	 * creates and holds tAppWindow instance;
	 * handles the simulation execution;
	 */
	class tApp: public QApplication {
		Q_OBJECT

	public://codetor

		tApp(int vArgC, char **vArgV);

		~tApp();

	public://actions

		void fRunSim(unsigned vSimBeingCount);

		void fRunEventKeyPress(QKeyEvent *vQKeyEvent);

	private://actions

		nSim::tBeing *fNewSimBeing(nSim::tBeing *vAncestor = nullptr);

	signals:

		void sRunSimCall();

		void sTryAntibioCall();

	public://varsdef

		nGui::tAppWindow *vWindow;
	};

}//namespace nApp

#endif//dChalGameOfLife_app_hpp
