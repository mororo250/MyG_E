#pragma once

#include "Foundation/Layer.h"

class LayerCollection
{
public:
	LayerCollection();
	~LayerCollection();

	void push_layer(Layer* layer);
	void pop_layer(Layer* layer);
	
	
	inline std::vector<Layer*>::iterator begin() { return m_layer_collection.begin(); }
	inline std::vector<Layer*>::iterator end() { return m_layer_collection.end(); }

	std::size_t count() { return m_layer_collection.size(); }

	Layer*& operator[](unsigned int index)
	{
		return m_layer_collection[index];
	}
private:
	std::vector<Layer*> m_layer_collection;
};