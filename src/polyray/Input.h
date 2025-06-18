#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#pragma once
#include <unordered_set>

class GLFWindow;

struct Input {
    static bool getKey(int key) {
        return keys.find(key) != keys.end();
    }

    static const std::unordered_set<int>& getActiveKeys() {
        return keys;
    }

private:
    inline static std::unordered_set<int> keys;

    static void setKey(int key, bool active) {
        if (active) keys.insert(key);
        else keys.erase(key);
    }

    friend class GLFWindow;
};

#endif
