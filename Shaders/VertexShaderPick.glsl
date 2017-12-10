#version 330

uniform mat4 m_pvm;

in vec4 position;

void main()
{
	gl_position = m_pvm * position;
}
