
attribute vec4 a_Position;

uniform mat4 u_MVMatrix;
uniform mat4 u_ProjectionMatrix;
uniform int u_Color;

varying vec4 v_Color;

void main()
{
	vec3 P = vec3(u_MVMatrix * a_Position);	
   	v_Color = vec4(u_Color/255.0, u_Color/255.0, u_Color/255.0, 1);
	gl_Position = u_ProjectionMatrix * vec4(P, 1.0);
}
