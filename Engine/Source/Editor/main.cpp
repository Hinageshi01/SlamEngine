#include "Editor.h"

int main()
{
	EditorInitor initor{ "Slam Engine", 1280, 720 };

	Editor *pApp = new Editor{ std::move(initor) };

	pApp->Update();

	delete pApp;

	return 0;
}
