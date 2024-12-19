#pragma once

#ifdef HAX_MONO
#include "../backends/haxsdk_mono.h"
#elif defined(HAX_IL2CPP)
#include "../backends/haxsdk_il2cpp.h"
#endif

namespace Unity {
    struct Object;
    struct Vector3;
    struct Transform;
    struct Component;
    struct GameObject;
    struct Camera;
    struct Screen;
    struct Light;
}

namespace HaxSdk {
    void InitializeUnityData();
}

struct Unity::Vector3 {
    static float        Distance(Unity::Vector3& a, Unity::Vector3& b);
public:
    float x;
    float y;
    float z;
};

struct Unity::Object : System::Object {
    static System::Array<Unity::Object*>* FindObjectsOfType(System::Type* type);
    static Unity::Object*                 FindObjectOfType(System::Type* type);
    static void                           Destroy(Unity::Object* obj);
public:
    System::String*                       get_name();
};

struct Unity::Component : Unity::Object {
    Unity::Transform* get_transform();
    Unity::GameObject* get_gameObject();
};

struct Unity::Transform : Unity::Component {
    Unity::Vector3      get_position();
    void                set_position(Unity::Vector3 value);
    Unity::Transform*   get_parent();
    void                set_parent(Unity::Transform* value);
};

struct Unity::GameObject : Unity::Object {
    static Unity::GameObject*  ctor();
    static Unity::GameObject*  ctor(const char* name);
    Unity::Transform*          get_transform();
    bool                       get_activeSelf();
    Unity::Component*          AddComponent(System::Type* componentType);
    void                       SetActive(bool value);
};

struct Unity::Camera {
    static Unity::Camera*      main();
    Unity::Vector3             WorldToScreenPoint(Unity::Vector3 position);
};

struct Unity::Screen {
    static int          width();
    static int          height();
};

struct Unity::Light {
    void                set_intensity(float value);
    float               get_intensity();
    void                set_range(float value);
    float               get_range();
};