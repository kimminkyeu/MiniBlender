//
// Created by Minkyeu Kim on 8/8/23.
//

#ifndef SCOOP_TESTSHADER_H
#define SCOOP_TESTSHADER_H

#include "Lunar/Shader/Shader.h"

class TestShader : public Lunar::Shader
{
public:
	TestShader()
			: Lunar::Shader("Test",
							"LunarApp/src/Shaders/Test/vertex_shader.glsl",
							"LunarApp/src/Shaders/Test/fragment_shader.glsl"
	)
	{};

	~TestShader() {};

private:
	 void OnBind() override {};
	 void OnUnbind() override {};
};

#endif//SCOOP_TESTSHADER_H
