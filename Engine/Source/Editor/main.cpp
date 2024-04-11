#include "Editor.h"

#include <memory>

int main()
{
	auto pApp = std::make_unique<Editor>();

	pApp->Init();

	return 0;
}
