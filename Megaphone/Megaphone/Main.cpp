#include "Main.h"

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:Main")

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Megaphone::Main form;
	Application::Run(% form);
}