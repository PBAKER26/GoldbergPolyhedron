#ifndef GOLDBERG_H
#define GOLDBERG_H

#include <SDL.h>
#include <stdio.h>
#include <GL/glew.h>
#include <fstream>
#include "cEvent.h"
#include "imageloader.h"
#include "math3d.h"
#include "polyhedron.h"

//Main class
class goldberg : public CEvent {
private:
	bool Running; bool Debug;
	SDL_Window * Window_Display = NULL;
	SDL_Surface * Surf_Display = NULL;
	
public:
	Vector3f rotation;						//rotation of the camera around the polyhedron
	int sizeM, sizeN;						//size of the polyhedron
	Polyhedron * polyModel;
	int debugInt;							//just for testing, determines how many faces of the polyhedron to hide

	goldberg();

	int OnExecute();
	bool OnInit();
	bool initRendering();

	void OnEvent(SDL_Event* Event);
	void OnKeyDown(SDL_Keycode sym, Uint16 mod);
	void OnKeyUp(SDL_Keycode sym, Uint16 mod);
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	void OnMouseBlur();
	void OnLButtonDown(int mX, int mY);
	void OnLButtonUp(int mX, int mY);
	void OnExit();

	void OnLoop();

	void OnRender();
	void drawScene();

	void OnCleanup();
};

#endif
