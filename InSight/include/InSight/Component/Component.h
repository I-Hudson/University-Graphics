#pragma once

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include "imgui.h"
#include "InSight/Shader/BaseShader.h"

class Component;
class Entity;

//ID is standard size t
//using ComponentID = std::size_t;

//
//inline ComponentID getNewComponentTypeID()
//{
//	static ComponentID lastID = 0u;
//	return lastID++;
//}

//get component type id
//template <typename T> inline ComponentID getComponentTypeID() noexcept
//{
//	static_assert (std::is_base_of<Component, T>::value, "");
//	static ComponentID typeID = getNewComponentTypeID();
//	return typeID;
//}
//
//constexpr std::size_t maxComponents = 32;

//array and bitset for componments
//using ComponentBitSet = std::bitset<maxComponents>;
//using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	//Owner
	Entity* entity;

	//virtual functions
	virtual void init() {}
	virtual void update() {}
	virtual void gui() {}
	virtual void draw(BaseShader* aShader = nullptr, const bool& aBindTextures = true) { aShader; aBindTextures; }
	virtual void draw(BaseShader* aShader = nullptr, const glm::mat4& aModelMatrx = glm::mat4(), const bool& aBindTextures = true) { aShader; aModelMatrx; aBindTextures; }
	virtual void destroy() {}
	virtual ~Component() {}

	virtual void Save(std::ofstream& aFile) = 0;

	//draw wireframe
	void drawWireFrame(bool aDrawWireFrame) { mDrawWireFrame = aDrawWireFrame; }
	//get wireframe status 
	bool* wireFrameStatus() { return &mDrawWireFrame; }

private:
	//wireframe
	bool mDrawWireFrame = false;
};

#endif // __COMPONENT_H__
