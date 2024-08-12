#include "exe.hpp"

namespace nExe {
int fRun(int vArgC, char **vArgV) {
  fmt::println("[exe]=(");
  fmt::println("[vArgC]={};", vArgC);
  fmt::println("[vArgV][0]={};", vArgV[0]);
  fmt::println(")=[exe]");
	return 0;
}
}//namespace nExe

int main(int vArgC, char **vArgV) {
	return nExe::fRun(vArgC, vArgV);
}
