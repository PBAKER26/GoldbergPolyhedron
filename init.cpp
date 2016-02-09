#include "goldberg.h"

bool goldberg::OnInit() {
	if (!initRendering()) return false;

	return true;
}
