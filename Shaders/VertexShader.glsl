
attribute vec4 a_Position;	        // in: Posición de cada vértice
attribute vec3 a_Normal;	        // in: Normal de cada vértice
attribute vec2 a_UV;                // in: Coordenadas UV de mapeado de textura

uniform mat4 u_ProjectionMatrix; 	// in: Matriz Projection
uniform mat4 u_MVMatrix;	        // in: Matriz ModelView
uniform mat4 u_VMatrix;             // in: Matriz View (cámara)

uniform vec4 LightPos0;
uniform vec4 LightPos1;
uniform vec4 LightPos2;

uniform int  u_Luz0;                // in: Indica si la luz 0 está encedida
uniform int  u_Luz1;
uniform int  u_Luz2;

varying vec4 v_Color;		        // out: Color al fragment shader
varying vec2 a_UV2;


void main()
{
    vec4 LightPos0 = u_VMatrix * LightPos0;
    vec4 LightPos1 = u_VMatrix * LightPos1;
    vec4 LightPos2 = u_VMatrix * LightPos2;

    vec3 P = vec3(u_MVMatrix * a_Position);	            // Posición del vértice
	vec3 N = vec3(u_MVMatrix * vec4(a_Normal, 0.0));    // Normal del vértice

	float d = length(LightPos0.xyz - P);			        // distancia de la luz
	vec3  L = normalize(LightPos0.xyz - P);			    // Vector Luz

	float d1 = length(LightPos1.xyz - P);			        // distancia de la luz
	vec3  L1 = normalize(LightPos1.xyz - P);			    // Vector Luz

	float d2 = length(LightPos2.xyz - P);			        // distancia de la luz
	vec3  L2 = normalize(LightPos2.xyz - P);			    // Vector Luz

	float ambient = 0.15;                               // (15% de int. ambiente)
	float diffuse = 0.0;
    float especular=0.0;

	if (u_Luz0>0) {                                     // Si la luz 0 está encendida se calcula la intesidad difusa de L
        diffuse = max(dot(N, L), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 75.0/(0.25+(0.01*d)+(0.003*d*d));
        diffuse *= attenuation;

        vec3 vReflection = reflect(-L,N);
        float spec = max(dot(N, vReflection), 0.0);
        especular = pow(spec, 32.0);
        especular *= d/50 * attenuation;
	}
    float c = ambient + diffuse + especular;

	if(u_Luz1>0){
        diffuse = max(dot(N, L1), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 75.0/(0.25+(0.01*d1)+(0.003*d1*d1));
        diffuse *= attenuation;

        vec3 vReflection = reflect(-L1,N);
        float spec = max(dot(N, vReflection), 0.0);
        especular = pow(spec, 32.0);
        especular *= attenuation;
	}
    c += diffuse + especular;

	if(u_Luz2>0){
        diffuse = max(dot(N, L2), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 75.0/(0.25+(0.01*d2)+(0.003*d2*d2));
        diffuse *= attenuation;

        vec3 vReflection = reflect(-L2,N);
        float spec = max(dot(N, vReflection), 0.0);
        especular = pow(spec, 32.0);
        especular *= attenuation;
	}

	a_UV2 = a_UV;
	//vec4 t_Color = texture2D(u_TextureUnit, a_UV);
	c += diffuse + especular;
	v_Color =  vec4(c,c,c,c);

	gl_Position = u_ProjectionMatrix * vec4(P, 1.0);
}
