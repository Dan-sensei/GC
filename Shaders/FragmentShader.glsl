
precision mediump float;	// Precisión media
uniform sampler2D u_TextureUnit;
varying vec4 v_Color;		// in: color del vertex shader
varying vec2 a_UV2;                // in: Coordenadas UV de mapeado de textura

void main()
{
    gl_FragColor = v_Color*texture2D(u_TextureUnit, a_UV2);
}
