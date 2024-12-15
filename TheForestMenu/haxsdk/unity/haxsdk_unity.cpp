#include "haxsdk_unity.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#define UNITY_CORE_ASSEMBLY "UnityEngine"

#define HAXSDK_FUNCTION(a, n, c, m, s)          static BackendMethod c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  static BackendMethod c ## __ ## m
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
    return reinterpret_cast<float(*)(Vector3, Vector3)>(Vector3__Distance.ptr)(a, b);
}

Array<BackendObject*>* Object::FindObjectsOfType(SystemType* type) {
    return reinterpret_cast<Array<BackendObject*>*(*)(SystemType*)>(Object__FindObjectsOfType.ptr)(type);
}

BackendObject* Object::FindObjectOfType(SystemType* type) {
    return reinterpret_cast<BackendObject*(*)(SystemType*)>(Object__FindObjectOfType.ptr)(type);
}

Vector3 Transform::get_position() {
    return *(Vector3*)Transform__get_position.pBase->invoke(this, nullptr)->unbox();
}

void Transform::set_position(Vector3 value) {
    void* args[1] = { &value };
    Transform__set_position.pBase->invoke(this, args);
}

Transform* Transform::get_parent() {
    return reinterpret_cast<Transform*(*)(Transform*)>(Transform__get_parent.ptr)(this);
}

void Transform::set_parent(Transform* value) {
    return reinterpret_cast<void(*)(Transform*, Transform*)>(Transform__set_parent.ptr)(this, value);
}

Transform* Component::get_transform() {
    return reinterpret_cast<Transform*(*)(Component*)>(Component__get_transform.ptr)(this);
}

Transform* GameObject::get_transform() {
    return reinterpret_cast<Transform*(*)(GameObject*)>(GameObject__get_transform.ptr)(this);
}

GameObject* GameObject::ctor() {
    GameObject* pObj = (GameObject*)BackendObject::alloc(BackendClass::find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
    GameObject__ctor1.pBase->invoke(pObj, nullptr);
    return pObj;
}

GameObject* GameObject::ctor(const char* name) {
    GameObject* pObj = (GameObject*)BackendObject::alloc(BackendClass::find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
    void* args[1] = { BackendString::alloc(name) };
    GameObject__ctor2.pBase->invoke(pObj, args);
    return pObj;
}

Component* GameObject::AddComponent(SystemType* componentType) {
    return reinterpret_cast<Component*(*)(GameObject*,SystemType*)>(GameObject__AddComponent.ptr)(this, componentType);
}

void GameObject::SetActive(bool value) {
    return reinterpret_cast<void(*)(GameObject*,bool)>(GameObject__SetActive.ptr)(this, value);
}

Camera* Camera::main() {
    return reinterpret_cast<Camera*(*)()>(Camera__get_main.ptr)();
}

Vector3 Camera::WorldToScreenPoint(Vector3 position) {
    return reinterpret_cast<Vector3(*)(Camera*,Vector3)>(Camera__WorldToScreenPoint.ptr)(this, position);
}

int Screen::width() {
    return reinterpret_cast<int(*)()>(Screen__get_width.ptr)();
}

int Screen::height() {
    return reinterpret_cast<int(*)()>(Screen__get_height.ptr)();
}

float Light::get_intensity() {
    return reinterpret_cast<float(*)(Light*)>(Light__get_intensity.ptr)(this);
}

void Light::set_intensity(float value) {
    void* args[1] = { &value };
    Light__set_intensity.pBase->invoke(this, args);
}

float Light::get_range() {
    return reinterpret_cast<float(*)(Light*)>(Light__get_range.ptr)(this);
}

void Light::set_range(float value) {
    void* args[1] = { &value };
    Light__set_range.pBase->invoke(this, args);
}