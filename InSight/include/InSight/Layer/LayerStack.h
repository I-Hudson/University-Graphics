#pragma once

#ifndef __LAYER_STACK_H__
#define __LAYER_STACK_H__

#include "Layer/Layer.h"

#include <vector>

class LayerStack
{
public:
	//Constructor
	LayerStack();
	//Destructor
	~LayerStack();

	//Push a new layer
	void PushLayer(Layer* aLayer);
	//Push a new overlay
	void PushOverlay(Layer* aOverlay);
	//Pop a layer
	void PopLayer(Layer* aLayer);
	//Pop an overlay
	void PopOverlay(Layer* aOverlay);

	std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
	std::vector<Layer*>::iterator end() { return mLayers.end(); }

private:
	//vector of all layers
	std::vector<Layer*> mLayers;
	unsigned int mLayerInsertIndex = 0;
};

#endif // __LAYER_STACK_H__
