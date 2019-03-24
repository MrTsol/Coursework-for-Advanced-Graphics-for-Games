#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/Window.h"
#include "Scenes.h"

int main() {
	Window w("CSC8502 Advanced Graphics for Games Coursework B8041320!", 1920, 1200, true);
	if (!w.HasInitialised()) {
		return -1;
	}

	Scenes scene(w);
	if (!scene.HasInitialised())
	{
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
		{
			scene.currentScene = 1;
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
		{
			scene.currentScene = 2;
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3))
		{
			scene.currentScene = 3;
		}

		scene.UpdateScene(w.GetTimer()->GetTimedMS());
		scene.RenderScene();
	}
	return 0;
}