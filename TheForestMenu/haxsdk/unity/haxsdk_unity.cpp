#include "haxsdk_unity.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#define UNITY_CORE_ASSEMBLY "UnityEngine"

#define HAXSDK_FUNCTION(a, n, c, m, s)          static BackendMethod* c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  static BackendMethod* c ## __ ## m
#define HAXSDK_STATIC_FIELD(a, n, c, f, t)      static t* c ## __ ## f
#define HAXSDK_FIELD_OFFSET(a, n, c, f)         static int c ## __ ## f
#include "../unity/haxsdk_unity_data.h"

void HaxSdk::InitializeUnityData() {
    #define HAXSDK_FUNCTION(a, n, c, m, s)          c ## __ ## m = BackendClass::find(a, n, #c)->find_method(#m, s)
    #define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  c ## __ ## m = BackendClass::find(a, n, #c)->find_method(mn, s)
    #define HAXSDK_STATIC_FIELD(a, n, c, f, t)      c ## __ ## f = (t*)BackendClass::find(a, n, #c)->find_static_field(#f)
    #define HAXSDK_FIELD_OFFSET(a, n, c, f)         c ## __ ## f = BackendClass::find(a, n, #c)->find_field(#f)->offset()
    #include "haxsdk_unity_data.h"
}

float Vector3::Distance(Vector3 a, Vector3 b) {
    return reinterpret_cast<float(*)(Vector3, Vector3)>(Vector3__Distance->address())(a, b);
}

Array<BackendObject*>* Object::FindObjectsOfType(SystemType* type) {
    return reinterpret_cast<Array<BackendObject*>*(*)(SystemType*)>(Object__FindObjectsOfType->address())(type);
}

BackendObject* Object::FindObjectOfType(SystemType* type) {
    return reinterpret_cast<BackendObject*(*)(SystemType*)>(Object__FindObjectOfType->address())(type);
}

Vector3 Transform::get_position() {
    return *(Vector3*)Transform__get_position->invoke(this, nullptr)->unbox();
}

void Transform::set_position(Vector3 value) {
    void* args[1] = { &value };
    Transform__set_position->invoke(this, args);
}

Transform* Transform::get_parent() {
    return reinterpret_cast<Transform*(*)(Transform*)>(Transform__get_parent->address())(this);
}

void Transform::set_parent(Transform* value) {
    return reinterpret_cast<void(*)(Transform*, Transform*)>(Transform__set_parent->address())(this, value);
}

Transform* Component::get_transform() {
    return reinterpret_cast<Transform*(*)(Component*)>(Component__get_transform->address())(this);
}

Transform* GameObject::get_transform() {
    return reinterpret_cast<Transform*(*)(GameObject*)>(GameObject__get_transform->address())(this);
}

GameObject* GameObject::ctor() {
    GameObject* pObj = (GameObject*)BackendObject::alloc(BackendClass::find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
    GameObject__ctor1->invoke(pObj, nullptr);
    return pObj;
}

GameObject* GameObject::ctor(const char* name) {
    GameObject* pObj = (GameObject*)BackendObject::alloc(BackendClass::find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
    void* args[1] = { BackendString::alloc(name) };
    GameObject__ctor2->invoke(pObj, args);
    return pObj;
}

Component* GameObject::AddComponent(SystemType* componentType) {
    return reinterpret_cast<Component*(*)(GameObject*,SystemType*)>(GameObject__AddComponent->address())(this, componentType);
}

void GameObject::SetActive(bool value) {
    return reinterpret_cast<void(*)(GameObject*,bool)>(GameObject__SetActive->address())(this, value);
}

Camera* Camera::main() {
    return reinterpret_cast<Camera*(*)()>(Camera__get_main->address())();
}

Vector3 Camera::WorldToScreenPoint(Vector3 position) {
    return reinterpret_cast<Vector3(*)(Camera*,Vector3)>(Camera__WorldToScreenPoint->address())(this, position);
}

int Screen::width() {
    return reinterpret_cast<int(*)()>(Screen__get_width->address())();
}

int Screen::height() {
    return reinterpret_cast<int(*)()>(Screen__get_height->address())();
}

float Light::get_intensity() {
    return reinterpret_cast<float(*)(Light*)>(Light__get_intensity->address())(this);
}

void Light::set_intensity(float value) {
    void* args[1] = { &value };
    Light__set_intensity->invoke(this, args);
    std::cout << Light__set_intensity->address() << '\n';
}

float Light::get_range() {
    return reinterpret_cast<float(*)(Light*)>(Light__get_range->address())(this);
}

void Light::set_range(float value) {
    void* args[1] = { &value };
    Light__set_range->invoke(this, args);
    std::cout << Light__set_range->address() << '\n';
}