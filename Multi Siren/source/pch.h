#pragma once

#include "plugin.h"

#include "CMessages.h"
#include "CFont.h"
#include "CSprite.h"
#include "CCoronas.h"
#include "CGeneral.h"
#include "CTimer.h"

#include "extensions/ScriptCommands.h"

#include "json/json.h"

#pragma comment(lib, "irrKlang.lib")
#include "irrKlang.h"
using namespace irrklang;

#include "map"

#include "log/Log.h"
#include "input/Input.h"
#include "menu/Menu.h"
//#include "localization/Localization.h"



using namespace plugin;


/*

sprintf(buffer, "%d", a);
MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);

*/

static CVector2D WorldToScreen(CVector position) {
	RwV3d rwp = { position.x, position.y, position.z };
	RwV3d screenCoors; float w, h;

	CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true);

	return CVector2D(screenCoors.x, screenCoors.y);
}

static void DrawWorldText(char* text, CVector position, CRGBA color = CRGBA(255, 255, 255, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5 * 0.7f, 1.0 * 0.7f);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	RwV3d rwp = { position.x, position.y, position.z };
	RwV3d screenCoors; float w, h;

	CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true);
	CFont::PrintString(screenCoors.x, screenCoors.y, text);
}

static void DrawScreenText(char* text, CVector2D position, CRGBA color = CRGBA(0, 255, 0, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5 * 0.7f, 1.0 * 0.7f);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	CFont::PrintString(position.x, position.y, text);
}

static void DrawScreenTextStr(std::string text, CVector2D position, CRGBA color = CRGBA(0, 255, 0, 255)) {
	char buffer[1024];
	sprintf(buffer, "%s", text.c_str());
	DrawScreenText(buffer, position, color);
}


static CVector CVectorLerp(CVector A, CVector B, float t) {
	return A * t + B * (1.f - t);
}

static CRGBA CRGBALerp(CRGBA color1, CRGBA color2, float t) {
	CRGBA color;
	color.r = (unsigned char)(color1.r + ((color2.r - color1.r) * t));
	color.g = (unsigned char)(color1.g + ((color2.g - color1.g) * t));
	color.b = (unsigned char)(color1.b + ((color2.b - color1.b) * t));
	color.a = 255;
	return color;
}

static float TotalDistanceBetweenPoints(std::vector<CVector> points) {
	float totalDistance = 0.0f;
	for (size_t i = 0; i < points.size() - 1; i++)
		totalDistance += DistanceBetweenPoints(points[i], points[i + 1]);
	return totalDistance;
}

static CVector PointInLines(std::vector<CVector> points, float position) {
	static char buffer[512];

	float totalDistance = TotalDistanceBetweenPoints(points);

	float distance = 0.0f;
	float toLerp = 0.0f;

	int point = -1;

	while (distance < position && point < (int)points.size() - 1)
	{
		point++;

		float len = DistanceBetweenPoints(points[point], points[point + 1]);

		toLerp = (position - distance) / len;

		//sprintf(buffer, "point %d; position %.2f; distance %.2f; len %.2f; toLerp %.2f", point, position, distance, len, toLerp);
		//MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);

		distance += len;
	}

	if (point == -1) return points[0];

	return CVectorLerp(points[point + 1], points[point], toLerp);
}

/*
static Json::Value ColorToJSON(CRGBA color) {
	Json::Value json;
	json["r"] = color.r;
	json["g"] = color.g;
	json["b"] = color.b;
	json["a"] = color.a;
	return json;
}

static CRGBA ColorFromJSON(Json::Value json) {
	CRGBA color;
	color.r = json["r"].asInt();
	color.g = json["g"].asInt();
	color.b = json["b"].asInt();
	color.a = json["a"].asInt();
	return color;
}

static Json::Value CVectorToJSON(CVector vec) {
	Json::Value json;
	json["x"] = vec.x;
	json["y"] = vec.y;
	json["z"] = vec.z;
	return json;
}

static CVector CVectorFromJSON(Json::Value json) {
	CVector vec;
	vec.x = json["x"].asFloat();
	vec.y = json["y"].asFloat();
	vec.z = json["z"].asFloat();
	return vec;
}
*/

static char* (*GetFrameNodeName)(RwFrame* frame) = (char* (*)(RwFrame*))0x72FB30;

static CVector GetFrameNodePosition(RwFrame* frame) {
	return { frame->modelling.pos.x, frame->modelling.pos.y, frame->modelling.pos.z };
}

static double GetAngleBetweenVectors(CVector v1, CVector v2, CVector v3) {
	double v12 = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	double v13 = sqrt(pow(v1.x - v3.x, 2) + pow(v1.y - v3.y, 2));
	double v23 = sqrt(pow(v2.x - v3.x, 2) + pow(v2.y - v3.y, 2));
	return acos((pow(v12, 2) + pow(v13, 2) - pow(v23, 2)) / (2 * (v12 * v13)));
}

static bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

static std::string to_upper(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::toupper(c);
		});
	return data;
}

static std::string to_lower(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
		});
	return data;
}

template <typename T>
static std::string join(const T& v, const std::string& delim) {
	std::ostringstream s;
	for (const auto& i : v) {
		if (&i != &v[0]) {
			s << delim;
		}
		s << i;
	}
	return s.str();
}