#include "haxsdk_unity.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#define UNITY_CORE_ASSEMBLY "UnityEngine"

#define HAXSDK_FUNCTION(a, n, c, m, s)          static Method c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  static Method c ## __ ## m
#define HAXSDK_STATIC_FIELD(a, n, c, f, t)      static t* c ## __ ## f
#define HAXSDK_FIELD_OFFSET(a, n, c, f)         static int c ## __ ## f
#include "../unity/haxsdk_unity_data.h"

void HaxSdk::InitializeUnityData() {
    #define HAXSDK_FUNCTION(a, n, c, m, s)          c ## __ ## m = Class::Find(a, n, #c)->FindMethod(#m, s)
    #define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  c ## __ ## m = Class::Find(a, n, #c)->FindMethod(mn, s)
    #define HAXSDK_STATIC_FIELD(a, n, c, f, t)      c ## __ ## f = (t*)Class::Find(a, n, #c)->FindStaticField(#f)
    #define HAXSDK_FIELD_OFFSET(a, n, c, f)         c ## __ ## f = Class::Find(a, n, #c)->FindField(#f)->Offset()
    #include "haxsdk_unity_data.h"
}

float Unity::Vector3::Distance(Unity::Vector3& a, Unity::Vector3& b) {
    Vector3 vector = { a.x - b.x, a.y - b.y, a.z - b.z };
    return std::sqrt(vector.x * vector.x + vector.y + vector.y + vector.z + vector.z);
}

System::Array<Unity::Object*>* Unity::Object::FindObjectsOfType(System::Type* type) {
    return reinterpret_cast<System::Array<Unity::Object*>*(*)(System::Type*)>(Object__FindObjectsOfType.Ptr())(type);
}

Unity::Object* Unity::Object::FindObjectOfType(System::Type* type) {
    return reinterpret_cast<Unity::Object*(*)(System::Type*)>(Object__FindObjectOfType.Ptr())(type);
}

void Unity::Object::Destroy(Unity::Object* obj) { 
    reinterpret_cast<void(*)(Unity::Object*,float)>(Object__Destroy.Ptr())(obj,0.F);
}

System::String* Unity::Object::get_name() {
    return reinterpret_cast<System::String*(*)(Unity::Object*)>(Object__get_name.Ptr())(this);
}

Unity::Vector3 Unity::Transform::get_position() {
    Vector3 res;
    reinterpret_cast<void(*)(Unity::Transform*,Unity::Vector3*)>(Transform__INTERNAL_get_position.Ptr())(this,&res);
    return res;
}

void Unity::Transform::set_position(Unity::Vector3 value) {
    reinterpret_cast<void(*)(Unity::Transform*,Unity::Vector3*)>(Transform__INTERNAL_set_position.Ptr())(this, &value);
}

Unity::Transform* Unity::Transform::get_parent() {
    return reinterpret_cast<Unity::Transform*(*)(Unity::Transform*)>(Transform__get_parent.Ptr())(this);
}

void Unity::Transform::set_parent(Unity::Transform* value) {
    return reinterpret_cast<void(*)(Transform*, Transform*)>(Transform__set_parent.Ptr())(this, value);
}

Unity::Transform* Unity::Component::get_transform() {
    return reinterpret_cast<Unity::Transform*(*)(Unity::Component*)>(Component__get_transform.Ptr())(this);
}

Unity::GameObject* Unity::Component::get_gameObject() {
    return reinterpret_cast<Unity::GameObject*(*)(Unity::Component*)>(Component__get_gameObject.Ptr())(this);
}

Unity::Transform* Unity::GameObject::get_transform() {
    return reinterpret_cast<Unity::Transform*(*)(Unity::GameObject*)>(GameObject__get_transform.Ptr())(this);
}

bool Unity::GameObject::get_activeSelf() {
    return reinterpret_cast<bool(*)(Unity::GameObject*)>(GameObject__get_activeSelf.Ptr())(this);
}

Unity::GameObject* Unity::GameObject::ctor() {
    return (Unity::GameObject*)System::Object::NewI(Class::Find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
}

Unity::GameObject* Unity::GameObject::ctor(const char* name) {
    Unity::GameObject* pObj = (GameObject*)Unity::GameObject::New(Class::Find(UNITY_CORE_ASSEMBLY, "UnityEngine", "GameObject"));
    void* args[1] = { System::String::New(name) };
    GameObject__ctor.Invoke(pObj, args);
    return pObj;
}

Unity::Component* Unity::GameObject::AddComponent(System::Type* componentType) {
    return reinterpret_cast<Component*(*)(GameObject*,System::Type*)>(GameObject__AddComponent.Ptr())(this, componentType);
}

void Unity::GameObject::SetActive(bool value) {
    return reinterpret_cast<void(*)(Unity::GameObject*,bool)>(GameObject__SetActive.Ptr())(this, value);
}

Unity::Camera* Unity::Camera::main() {
    return reinterpret_cast<Unity::Camera*(*)()>(Camera__get_main.Ptr())();
}

Unity::Vector3 Unity::Camera::WorldToScreenPoint(Unity::Vector3 position) {
    Vector3 res;
    reinterpret_cast<void(*)(Unity::Camera*,Unity::Vector3*,Unity::Vector3*)>(Camera__INTERNAL_CALL_WorldToScreenPoint.Ptr())(this, &position, &res);
    return res;
}

int Unity::Screen::width() {
    return reinterpret_cast<int(*)()>(Screen__get_width.Ptr())();
}

int Unity::Screen::height() {
    return reinterpret_cast<int(*)()>(Screen__get_height.Ptr())();
}

float Unity::Light::get_intensity() {
    return reinterpret_cast<float(*)(Unity::Light*)>(Light__get_intensity.Ptr())(this);
}

void Unity::Light::set_intensity(float value) {
    reinterpret_cast<void(*)(Unity::Light*,float)>(Light__set_intensity.Ptr())(this, value);
}

float Unity::Light::get_range() {
    return reinterpret_cast<float(*)(Unity::Light*)>(Light__get_range.Ptr())(this);
}

void Unity::Light::set_range(float value) {
    return reinterpret_cast<void(*)(Unity::Light*,float)>(Light__set_range.Ptr())(this, value);
}