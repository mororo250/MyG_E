#include "LayerCollection.h"

LayerCollection::LayerCollection()
{
}

LayerCollection::~LayerCollection()
{
	for (auto* aux : m_layer_collection)
		delete aux;
}

void LayerCollection::push_layer(Layer* layer)
{
	m_layer_collection.emplace_back(layer);
}

void LayerCollection::pop_layer(Layer* layer)
{
	auto aux = std::find(m_layer_collection.begin(), m_layer_collection.end(), layer);
	if (aux != m_layer_collection.end())
		m_layer_collection.erase(aux);
}
