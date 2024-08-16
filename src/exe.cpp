#include "exe.hpp"

#include "sim.hpp"
#include "app.hpp"
#include "gui.hpp"

int main(int vArgC, char **vArgV) {
	fmt::println(stderr, "[main]=(");
	try {
		fLogErr("vArgC", "{};", vArgC);
		fLogErr("vArgV[0]", "\"{}\";", vArgV[0]);
		fLogErr("dProjName", "\"{}\";", dProjName);
		fLogErr("dWorkPath", "\"{}\";", std::filesystem::current_path().c_str());
		fLogErr("QT_VERSION_STR", "\"{}\";", QT_VERSION_STR);
		{
			auto vApp = nApp::tApp(vArgC, vArgV);
			auto vErr = vApp.exec();
			fLogErr("error", "{};", vErr);
			if(vErr) {
				return vErr;
			}
		}
	} catch(const std::exception &vError) {
		fLogErr("[exception]", "\"{}\";", vError.what());
		return 1;
	}
	fmt::println(stderr, ")=[main]");
	return 0;
}
