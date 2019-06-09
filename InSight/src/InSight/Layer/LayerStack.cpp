#include "Layer/LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : mLayers)
	{
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* aLayer)
{
	mLayers.emplace(mLayers.begin() + mLayerInsertIndex, aLayer);
	mLayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* aOverlay)
{
	mLayers.emplace_back(aOverlay);
}

void LayerStack::PopLayer(Layer* aLayer)
{
	auto it = std::find(mLayers.begin(), mLayers.end(), aLayer);
	if (it != mLayers.end())
	{
		mLayers.erase(it);
		mLayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* aOverlay)
{
	auto it = std::find(mLayers.begin(), mLayers.end(), aOverlay);
	if (it != mLayers.end())
	{
		mLayers.erase(it);
	}
}
