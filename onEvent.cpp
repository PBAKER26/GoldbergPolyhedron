#include "goldberg.h"

void goldberg::OnEvent(SDL_Event* Event) {
	CEvent::OnEvent(Event);
}

void goldberg::OnExit() {
	Running = false;
}

void goldberg::OnKeyDown(SDL_Keycode sym, Uint16 mod) {
	switch (sym) {
		case SDLK_UP:		{ sizeM++;					delete polyModel; polyModel = new Polyhedron(sizeM, sizeN); break; }
		case SDLK_DOWN:		{ if (sizeM > 1) sizeM--;	delete polyModel; polyModel = new Polyhedron(sizeM, sizeN); break; }
		case SDLK_LEFT:		{ sizeN++;					delete polyModel; polyModel = new Polyhedron(sizeM, sizeN); break; }
		case SDLK_RIGHT:	{ if (sizeN > 0) sizeN--;	delete polyModel; polyModel = new Polyhedron(sizeM, sizeN); break; }

		//for testing purposes, hides faces of polyhedron
		case SDLK_LEFTBRACKET: { if (debugInt > 0) debugInt--; break;}
		case SDLK_RIGHTBRACKET: { debugInt++; break;}
	}
}

void goldberg::OnKeyUp(SDL_Keycode sym, Uint16 mod) {
	
}

void goldberg::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
	if (Left) {
		rotation.x -= relY;
		rotation.y += relX;

	}
}

void goldberg::OnMouseBlur() {

}

void goldberg::OnLButtonDown(int mX, int mY) {
	
}

void goldberg::OnLButtonUp(int mX, int mY) {
	
}
