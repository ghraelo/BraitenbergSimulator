//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

// Source altered and distributed from https://github.com/AdrienHerubel/imgui

#include <stdio.h>
#include <string.h>
#include <algorithm>
//#define _USE_MATH_DEFINES
#include <math.h>
#include "imgui.h"
#include <nanovg\nanovg.h>
#include "GUIRenderer.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GuiState
{
	GuiState()
	{
		left = false;
		leftPressed = false;
		leftReleased = false;
		mx = -1;
		my = -1;
		scroll = 0;
		active = 0;
		hot = 0;
		hotToBe = 0;
		isHot = false;
		isActive = false;
		wentActive = false;
		dragX = 0;
		dragY = 0;
		dragOrig = 0;
		widgetX = 0;
		widgetY = 0;
		widgetW = 100;
		insideCurrentScroll = false;
		areaId = 0;
		widgetId = 0;
	}

	bool left;
	bool leftPressed, leftReleased;
	int mx, my;
	int scroll;
	unsigned int active;
	unsigned int hot;
	unsigned int hotToBe;
	bool isHot;
	bool isActive;
	bool wentActive;
	int dragX, dragY;
	float dragOrig;
	int widgetX, widgetY, widgetW;
	bool insideCurrentScroll;

	unsigned int areaId;
	unsigned int widgetId;
};

static GuiState s_state;
static GUIRenderer* g_rend;

inline bool anyActive()
{
	return s_state.active != 0;
}

inline bool isActive(unsigned int id)
{
	return s_state.active == id;
}

inline bool isHot(unsigned int id)
{
	return s_state.hot == id;
}

inline bool inRect(int x, int y, int w, int h, bool checkScroll = true)
{
	return (!checkScroll || s_state.insideCurrentScroll) && s_state.mx >= x && s_state.mx <= x + w && s_state.my >= y && s_state.my <= y + h;
}

inline void clearInput()
{
	s_state.leftPressed = false;
	s_state.leftReleased = false;
	s_state.scroll = 0;
}

inline void clearActive()
{
	s_state.active = 0;
	// mark all UI for this frame as processed
	clearInput();
}

inline void setActive(unsigned int id)
{
	s_state.active = id;
	s_state.wentActive = true;
}

inline void setHot(unsigned int id)
{
	s_state.hotToBe = id;
}


static bool buttonLogic(unsigned int id, bool over)
{
	bool res = false;
	// process down

	if (!anyActive())
	{
		if (over)
		{
			setHot(id);
		}

		if (isHot(id) && s_state.leftPressed)
		{
			setActive(id);
		}
	}

	// if button is active, then react on left up
	if (isActive(id))
	{
		s_state.isActive = true;
		if (over)
			setHot(id);
		if (s_state.leftReleased)
		{
			if (isHot(id))
				res = true;
			clearActive();
		}
	}

	if (isHot(id))
		s_state.isHot = true;

	return res;
}

static void updateInput(int mx, int my, unsigned char mbut, int scroll)
{
	bool left = (mbut & IMGUI_MBUT_LEFT) != 0;

	s_state.mx = mx;
	s_state.my = my;
	s_state.leftPressed = !s_state.left && left;
	s_state.leftReleased = s_state.left && !left;
	s_state.left = left;

	s_state.scroll = scroll;
}

void imguiBeginFrame(int mx, int my, unsigned char mbut, int scroll, GUIRenderer* gRenderer)
{
	updateInput(mx, my, mbut, scroll);

	s_state.hot = s_state.hotToBe;
	s_state.hotToBe = 0;

	s_state.wentActive = false;
	s_state.isActive = false;
	s_state.isHot = false;

	s_state.widgetX = 0;
	s_state.widgetY = 0;
	s_state.widgetW = 0;

	s_state.areaId = 1;
	s_state.widgetId = 1;

	g_rend = gRenderer;
}

void imguiEndFrame()
{
	clearInput();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int BUTTON_HEIGHT = 20;
static const int SLIDER_HEIGHT = 20;
static const int BAR_HEIGHT = 20;
static const int SLIDER_MARKER_WIDTH = 10;
static const int CHECK_SIZE = 8;
static const int DEFAULT_SPACING = 4;
static const int TEXT_HEIGHT = 16;
static const int SCROLL_AREA_PADDING = 6;
static const int INDENT_SIZE = 16;
static const int AREA_HEADER = 28;

static int g_scrollTop = 0;
static int g_scrollBottom = 0;
static int g_scrollRight = 0;
static int g_scrollAreaTop = 0;
static int* g_scrollVal = 0;
static int g_focusTop = 0;
static int g_focusBottom = 0;
static unsigned int g_scrollId = 0;
static bool g_insideScrollArea = false;

bool imguiBeginScrollArea(const char* name, int x, int y, int w, int h, int* scroll)
{
	s_state.areaId++;
	s_state.widgetId = 0;
	g_scrollId = (s_state.areaId << 16) | s_state.widgetId;

	s_state.widgetX = x + SCROLL_AREA_PADDING;
	s_state.widgetY = y + AREA_HEADER - (*scroll);
	s_state.widgetW = w - SCROLL_AREA_PADDING * 4;
	g_scrollTop = y + AREA_HEADER;
	g_scrollBottom = y + h - SCROLL_AREA_PADDING;
	g_scrollRight = x + w - SCROLL_AREA_PADDING * 3;
	g_scrollVal = scroll;

	g_scrollAreaTop = s_state.widgetY;

	g_focusTop = y + AREA_HEADER;
	g_focusBottom = y + h - AREA_HEADER;

	g_insideScrollArea = inRect(x, y, w, h, false);
	s_state.insideCurrentScroll = g_insideScrollArea;

	g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 6, nvgRGBA(0, 0, 0, 192));

	g_rend->AddGfxCmdText(x + AREA_HEADER / 2 + s_state.widgetW / 2, y + AREA_HEADER / 2 - TEXT_HEIGHT / 2,  name, nvgRGBA(255, 255, 255, 128));
	//g_rend->AddGfxCmdRect(x + SCROLL_AREA_PADDING, y + AREA_HEADER, w - SCROLL_AREA_PADDING * 4, h - AREA_HEADER - SCROLL_AREA_PADDING, nvgRGBA(155, 0, 0, 192));
	g_rend->AddGfxCmdScissor(x + SCROLL_AREA_PADDING, y + AREA_HEADER, w - SCROLL_AREA_PADDING * 4, h - AREA_HEADER - SCROLL_AREA_PADDING);

	return g_insideScrollArea;
}

void imguiEndScrollArea()
{
	// Disable scissoring.
	g_rend->AddGfxCmdScissor(-1, -1, -1, -1);

	// Draw scroll bar
	int x = g_scrollRight + SCROLL_AREA_PADDING / 2;
	int y = g_scrollTop;
	int w = SCROLL_AREA_PADDING * 2;
	int h = g_scrollBottom - g_scrollTop;

	int stop = g_scrollAreaTop;
	int sbot = s_state.widgetY;
	int sh = sbot - stop; // The scrollable area height.
	//printf("sh: %d\n", sh);
	float barHeight = (float)h / (float)sh;

	if (barHeight < 1)
	{
		float barY = (float)(y - stop) / (float)sh;
		//printf("barY: %f\n", barY);
		if (barY < 0) barY = 0;
		if (barY > 1) barY = 1;

		// Handle scroll bar logic.
		unsigned int hid = g_scrollId;
		int hx = x;
		int hy = y + (int)(barY*h);
		int hw = w;
		int hh = (int)(barHeight*h);

		const int range = h - (hh - 1);
		bool over = inRect(hx, hy, hw, hh);
		buttonLogic(hid, over);
		if (isActive(hid))
		{
			float u = (float)(hy - y) / (float)range;
			if (s_state.wentActive)
			{
				s_state.dragY = s_state.my;
				s_state.dragOrig = u;
			}
			if (s_state.dragY != s_state.my)
			{
				u = s_state.dragOrig + (s_state.dragY - s_state.my) / (float)range;
				if (u < 0) u = 0;
				if (u > 1) u = 1;
				*g_scrollVal = (int)((1 - u) * (sh - h));
				//printf("scrollval: %d\n", *g_scrollVal);
			}
		}

		// BG
		g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, (float)w / 2 - 1, nvgRGBA(0, 0, 0, 196));
		// Bar
		if (isActive(hid))
			g_rend->AddGfxCmdRoundedRect((float)hx, (float)hy, (float)hw, (float)hh, (float)w / 2 - 1, nvgRGBA(255, 196, 0, 196));
		else
			g_rend->AddGfxCmdRoundedRect((float)hx, (float)hy, (float)hw, (float)hh, (float)w / 2 - 1, isHot(hid) ? nvgRGBA(255, 196, 0, 96) : nvgRGBA(255, 255, 255, 64));

		// Handle mouse scrolling.
		if (g_insideScrollArea) // && !anyActive())
		{
			if (s_state.scroll)
			{
				*g_scrollVal += 20 * s_state.scroll;
				if (*g_scrollVal < 0) *g_scrollVal = 0;
				if (*g_scrollVal >(sh - h)) *g_scrollVal = (sh - h);
			}
		}
	}
	s_state.insideCurrentScroll = false;
}

bool imguiButton(const char* text, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;
	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = BUTTON_HEIGHT;
	s_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && inRect(x, y, w, h);

	bool res = buttonLogic(id, over);

	g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, (float)BUTTON_HEIGHT / 2 - 1, nvgRGBA(128, 128, 128, isActive(id) ? 196 : 96));
	if (enabled)
		g_rend->AddGfxCmdText(x + w / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
	else
		g_rend->AddGfxCmdText(x + w / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(128, 128, 128, 200));

	return res;
}

void imguiBarDisplay(std::vector<Interval> intervals)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = BAR_HEIGHT;
	s_state.widgetY += BAR_HEIGHT + DEFAULT_SPACING;

	g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, (float)BUTTON_HEIGHT / 4 - 1, nvgRGBA(128, 128, 128, 96));

	if (intervals.size() > 0)
	{
		for (auto& interval : intervals)
		{
			
			if(interval.first != interval.second)
				g_rend->AddGfxCmdRect((float)x + w*interval.first, (float)y, (float)w*(interval.second - interval.first), (float)h, nvgRGBA(128, 128, 128, 196));
		}
	}
}

void imguiGraphDisplay(std::vector<float> data)
{
	float max = *std::max_element(data.begin(), data.end());
	float min = *std::min_element(data.begin(), data.end());
	imguiGraphDisplay(data, min, max);
}

void imguiGraphDisplay(std::vector<float> data, float min, float max)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int y_mid = s_state.widgetY + 2.5 * BUTTON_HEIGHT;
	int w = s_state.widgetW;
	int w2 = 1;
	int h = 5 * BUTTON_HEIGHT;
	int h2 = 1;
	s_state.widgetY += 5 * BUTTON_HEIGHT + DEFAULT_SPACING;

	//background
	g_rend->AddGfxCmdRect((float)x, (float)y, (float)w, (float)h, nvgRGBA(128, 128, 128, 96));

	//axes
	g_rend->AddGfxCmdRect((float)x, (float)y_mid, (float)w, (float)h2, nvgRGBA(255, 255, 255, 32));
	g_rend->AddGfxCmdRect((float)x, (float)y, (float)w2, (float)h, nvgRGBA(255, 255, 255, 32));

	//downsample data
	std::vector<std::pair<float,float>> downSampled;


	int dsRate = floor((data.size()-2)/(s_state.widgetW / 4));
	int dsLength = data.size() - (dsRate + 1);
	downSampled.reserve(dsLength);

	float scaling = (h / 2) / fmax(fabs(min), fabs(max));
	float spacing = w / dsLength;

	std::pair<float, float> startPt(x, y_mid + data[0] * scaling);

	downSampled.push_back(startPt);

	for (int i = 1; i < dsLength; i += dsRate)
	{
		float temp = 0;
		for (int j = 0; j < dsRate; j++)
		{
			temp += data[i + j];
		}
		temp = temp / dsRate;
		std::pair<float, float> pt(x + spacing*i, y_mid + temp*scaling);
;		downSampled.push_back(pt);
	}

	std::pair<float, float> endPt(x + dsLength*spacing, y_mid + *(data.end() - 1) * scaling);
	downSampled.push_back(endPt);

	g_rend->AddGfxCmdPolyLine(downSampled,1, nvgRGBA(255, 196, 0, 255));

}

bool imguiItem(const char* text, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = BUTTON_HEIGHT;
	s_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && inRect(x, y, w, h);
	bool res = buttonLogic(id, over);

	if (isHot(id))
		g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 2.0f, nvgRGBA(255, 196, 0, isActive(id) ? 196 : 96));

	if (enabled)
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(255, 255, 255, 200));
	else
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(128, 128, 128, 200));

	return res;
}

bool imguiCheck(const char* text, bool checked, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = BUTTON_HEIGHT;
	s_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && inRect(x, y, w, h);
	bool res = buttonLogic(id, over);

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	g_rend->AddGfxCmdRoundedRect((float)cx - 3, (float)cy - 3, (float)CHECK_SIZE + 6, (float)CHECK_SIZE + 6, 4, nvgRGBA(128, 128, 128, isActive(id) ? 196 : 96));
	if (checked)
	{
		if (enabled)
			g_rend->AddGfxCmdRoundedRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, (float)CHECK_SIZE / 2 - 1, nvgRGBA(255, 255, 255, isActive(id) ? 255 : 200));
		else
			g_rend->AddGfxCmdRoundedRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, (float)CHECK_SIZE / 2 - 1, nvgRGBA(128, 128, 128, 200));
	}

	if (enabled)
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, NVG_ALIGN_LEFT, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
	else
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, text, NVG_ALIGN_LEFT, nvgRGBA(128, 128, 128, 200));

	return res;
}

bool imguiCollapse(const char* text, const char* subtext, bool checked, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY - BUTTON_HEIGHT;
	int w = s_state.widgetW;
	int h = BUTTON_HEIGHT;
	s_state.widgetY -= BUTTON_HEIGHT; // + DEFAULT_SPACING;

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;

	bool over = enabled && inRect(x, y, w, h);
	bool res = buttonLogic(id, over);

	if (checked)
		g_rend->AddGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 2, nvgRGBA(255, 255, 255, isActive(id) ? 255 : 200));
	else
		g_rend->AddGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 1, nvgRGBA(255, 255, 255, isActive(id) ? 255 : 200));

	if (enabled)
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
	else
		g_rend->AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(128, 128, 128, 200));

	if (subtext)
		g_rend->AddGfxCmdText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  subtext, nvgRGBA(255, 255, 255, 128));

	return res;
}

void imguiLabel(const char* text)
{
	imguiLabel(text, false);
}

void imguiLabel(const char * text, bool highlight)
{
	int x = s_state.widgetX;
	int y = s_state.widgetY;
	s_state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;
	g_rend->AddGfxCmdText(x, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, text, NVG_ALIGN_LEFT, highlight ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 255));
}

void imguiValue(const char* text)
{
	const int x = s_state.widgetX;
	const int y = s_state.widgetY;
	const int w = s_state.widgetW;
	s_state.widgetY += BUTTON_HEIGHT;

	g_rend->AddGfxCmdText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(255, 255, 255, 200));
}

bool imguiSlider(const char* text, float* val, float vmin, float vmax, float vinc, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = SLIDER_HEIGHT;
	s_state.widgetY += SLIDER_HEIGHT + DEFAULT_SPACING;

	g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 4.0f, nvgRGBA(0, 0, 0, 128));

	const int range = w - SLIDER_MARKER_WIDTH;

	float u = (*val - vmin) / (vmax - vmin);
	if (u < 0) u = 0;
	if (u > 1) u = 1;
	int m = (int)(u * range);

	bool over = enabled && inRect(x + m, y, SLIDER_MARKER_WIDTH, SLIDER_HEIGHT);
	bool res = buttonLogic(id, over);
	bool valChanged = false;

	if (isActive(id))
	{
		if (s_state.wentActive)
		{
			s_state.dragX = s_state.mx;
			s_state.dragOrig = u;
		}
		if (s_state.dragX != s_state.mx)
		{
			u = s_state.dragOrig + (float)(s_state.mx - s_state.dragX) / (float)range;
			if (u < 0) u = 0;
			if (u > 1) u = 1;
			*val = vmin + u*(vmax - vmin);
			*val = floorf(*val / vinc + 0.5f)*vinc; // Snap to vinc
			m = (int)(u * range);
			valChanged = true;
		}
	}

	if (isActive(id))
		g_rend->AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, nvgRGBA(255, 255, 255, 255));
	else
		g_rend->AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, isHot(id) ? nvgRGBA(255, 196, 0, 128) : nvgRGBA(255, 255, 255, 64));

	// TODO: fix this, take a look at 'nicenum'.
	int digits = (int)(ceilf(log10f(vinc)));
	char fmt[16];
	snprintf(fmt, 16, "%%.%df", digits >= 0 ? 0 : -digits);
	char msg[128];
	snprintf(msg, 128, fmt, *val);

	if (enabled)
	{
		g_rend->AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
		g_rend->AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  msg, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
	}
	else
	{
		g_rend->AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(128, 128, 128, 200));
		g_rend->AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  msg, nvgRGBA(128, 128, 128, 200));
	}

	return res || valChanged;
}


bool imguiSlider(const char* text, int* val, int vmin, int vmax, int vinc, bool enabled)
{
	s_state.widgetId++;
	unsigned int id = (s_state.areaId << 16) | s_state.widgetId;

	int x = s_state.widgetX;
	int y = s_state.widgetY;
	int w = s_state.widgetW;
	int h = SLIDER_HEIGHT;
	s_state.widgetY += SLIDER_HEIGHT + DEFAULT_SPACING;

	g_rend->AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 4.0f, nvgRGBA(0, 0, 0, 128));

	const int range = w - SLIDER_MARKER_WIDTH;

	float u = (*val - vmin) / float(vmax - vmin);
	if (u < 0) u = 0;
	if (u > 1) u = 1;
	int m = (int)(u * range);

	bool over = enabled && inRect(x + m, y, SLIDER_MARKER_WIDTH, SLIDER_HEIGHT);
	bool res = buttonLogic(id, over);
	bool valChanged = false;

	if (isActive(id))
	{
		if (s_state.wentActive)
		{
			s_state.dragX = s_state.mx;
			s_state.dragOrig = u;
		}
		if (s_state.dragX != s_state.mx)
		{
			u = s_state.dragOrig + (float)(s_state.mx - s_state.dragX) / (float)range;
			if (u < 0) u = 0;
			if (u > 1) u = 1;
			*val = int(vmin + u*(vmax - vmin));
			*val = int(floorf(*val / float(vinc) + 0.5f))*vinc; // Snap to vinc
			m = (int)(u * range);
			valChanged = true;
		}
	}

	if (isActive(id))
		g_rend->AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, nvgRGBA(255, 255, 255, 255));
	else
		g_rend->AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, isHot(id) ? nvgRGBA(255, 196, 0, 128) : nvgRGBA(255, 255, 255, 64));

	char msg[128];
	snprintf(msg, 128, "%d", *val);

	if (enabled)
	{
		g_rend->AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
		g_rend->AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  msg, isHot(id) ? nvgRGBA(255, 196, 0, 255) : nvgRGBA(255, 255, 255, 200));
	}
	else
	{
		g_rend->AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  text, nvgRGBA(128, 128, 128, 200));
		g_rend->AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2,  msg, nvgRGBA(128, 128, 128, 200));
	}

	return res || valChanged;
}


void imguiIndent()
{
	s_state.widgetX += INDENT_SIZE;
	s_state.widgetW -= INDENT_SIZE;
}

void imguiUnindent()
{
	s_state.widgetX -= INDENT_SIZE;
	s_state.widgetW += INDENT_SIZE;
}

void imguiSeparator()
{
	s_state.widgetY += DEFAULT_SPACING * 3;
}

void imguiSeparatorLine()
{
	int x = s_state.widgetX;
	int y = s_state.widgetY + DEFAULT_SPACING * 1.5;
	int w = s_state.widgetW;
	int h = 1;
	s_state.widgetY += DEFAULT_SPACING * 4;

	g_rend->AddGfxCmdLine((float)x, (float)y, x+ w, y,0, nvgRGBA(255, 255, 255, 132));
}