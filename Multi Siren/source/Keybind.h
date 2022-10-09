#pragma once

#include "pch.h"

//key; alt; shift; ctrl
struct Keybind {
    int key;
    bool alt;
    bool shift;
    bool ctrl;
    //bool code = false;  if it loads A, saves A, if it loads 65 saves 65 (to json)

    /*
    Json::Value ToJSON()
    {
        Json::Value value = Json::objectValue;

        if (key == -1) {
            value["key"] = "";
        }
        else
        {
            if (code) value["key"] = key;
            else value["key"] = GetKeyString();
        }

        value["alt"] = alt;
        value["shift"] = shift;
        value["ctrl"] = ctrl;

        return value;
    }
    */

    /*
    void FromJSON(Json::Value value)
    {
        if (value.empty()) return;

        bool keyIsNumber = value["key"].isNumeric();
        code = value["key"].isNumeric();

        if (keyIsNumber)
        {
            SetKey(value["key"].asInt());

            key = value["key"].asInt();
        }
        else {
            auto keyStr = ToUpper(value["key"].asString());

            if (keyStr.empty()) key = -1;
            else key = (int)keyStr.at(0);
        }

        alt = value["alt"].asBool();
        shift = value["shift"].asBool();
        ctrl = value["ctrl"].asBool();
    }
    */

    void SetKey(std::string str)
    {
        if (is_number(str))
        {
            key = std::atoi(str.c_str());
        }
        else
        {
            key = (int)str.at(0);
        }
    }

    /*
    void SetKey(int key)
    {
        code = true;
    }
    */

    
    std::string GetKeybindString()
    {
        std::vector<std::string> ks;

        if (ctrl) ks.push_back("CTRL");
        if (shift) ks.push_back("SHIFT");
        if (alt) ks.push_back("ALT");

        if (ks.size() == 0) return GetKeyString();

        if (key != -1) ks.push_back(GetKeyString());

        return join(ks, " + ");
    }
    
    std::string GetKeyString()
    {
        if (key == -1) return "";

        char ch = static_cast<char>(key);
        return to_upper(std::string(1, ch));
    }

    bool CheckKeybind()
    {
        if (alt && !Input::GetKey(18)) return false;
        if (ctrl && !Input::GetKey(17)) return false;
        if (shift && !Input::GetKey(16)) return false;

        if (key == -1) return true;

        return Input::GetKeyDown(key);
    }
};