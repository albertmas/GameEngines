#ifndef _PANEL_CONFIGURATION_H_
#define _PANEL_CONFIGURATION_H_
#include "Panel.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "ImGui/imgui_internal.h"


class Panel_Configuration :
	public Panel
{
public:
	float fps_log[32];
	float mms_log[32];
	static char app_name_input;
	static char app_org_input;

public:
	Panel_Configuration();
	virtual ~Panel_Configuration();
	void Draw()override;
	void Application();



};

#endif