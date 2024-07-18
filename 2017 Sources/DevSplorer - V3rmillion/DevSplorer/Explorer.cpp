#include "Explorer.h"
using namespace DevSplorer;
using namespace std;

BOOL explorerinit() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Explorer^ newexplorer = gcnew Explorer();
	Application::Run(newexplorer);
	return 1;
}