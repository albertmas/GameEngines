#ifndef _PANEL_SCENE_
#define _PANEL_SCENE_

#include "Panel.h"
class Panel_Scene : public Panel
{
public:
	Panel_Scene();
	virtual ~Panel_Scene();
	void Draw()override;
};

#endif