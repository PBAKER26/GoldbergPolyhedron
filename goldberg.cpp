// This is a program with barebones opengl sdl graphic engine to display a goldberg polyhedron
// Ive borrowed bits of code for the 3d graphics portion from various free internet tutorials.

#include "goldberg.h"

goldberg::goldberg() {
	Running = true;
	rotation = Vector3f(0, 0, 0);
	sizeM = 1; sizeN = 1;
	debugInt = 0;
}

int goldberg::OnExecute() {
	if (OnInit() == false) return -1;
	SDL_Event Event;
	while (Running) {
		while (SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}
		OnRender();
	}
	return 0;
}

int main(int argc, char* args[]) {
	goldberg Main;
	return Main.OnExecute();
}
