//
// Created by USER on 2023-08-10.
//

#include "DataVisualizer.h"
DataVisualizer::DataVisualizer()
{
}

DataVisualizer::~DataVisualizer()
{
	delete m_PolygonShader;
	delete m_VerticesShader;
	delete m_NormalShader;
}

void DataVisualizer::Init()
{
    m_NormalShader = new NormalShader();
    m_VerticesShader = new PointShader();
    m_PolygonShader = new WireframeShader();
}