#include "Layer/LayerStack.h"

LayerStack::LayerStack()
{
	mLayerInsert = mLayers.begin();
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
	mLayerInsert = mLayers.emplace(mLayerInsert, aLayer);
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
		mLayerInsert--;
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
