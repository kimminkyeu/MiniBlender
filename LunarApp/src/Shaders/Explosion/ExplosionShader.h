//
// Created by Minkyeu Kim on 8/8/23.
//

#ifndef SCOOP_EXPLOSIONSHADER_H
#define SCOOP_EXPLOSIONSHADER_H

// 해당 쉐이더에 관련된 변수는 여기에서 저장.
#include "Lunar/Shader/Shader.h"

class ExplosionShader : public Lunar::Shader
{
public:
	float m_Degree = 0.0f; // Explosion degree
private:
	GLint m_DegreeLocation = 0;
public:
	ExplosionShader()
		: Lunar::Shader("Explosion",
									"LunarApp/src/Shaders/Explosion/vertex_shader.glsl",
									"LunarApp/src/Shaders/Explosion/fragment_shader.glsl",
									"LunarApp/src/Shaders/Explosion/geometry_shader.glsl"
						)
	{
		m_DegreeLocation = this->_GetUniformLocation("ExplosionDegree");
	}

private:
	void OnBind() override
	{
		glUniform1f(m_DegreeLocation, m_Degree);
	}

	void OnUnbind() override {};
};


#endif//SCOOP_EXPLOSIONSHADER_H
