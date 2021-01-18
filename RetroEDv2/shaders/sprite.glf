#version 330 core
in vec2 TexCoords;
out vec4 fragColour;

uniform sampler2D sprite;

uniform bool flipX;
uniform bool flipY;
uniform bool useAlpha;

uniform float alpha;
uniform vec3 transparentColour;

void main()
{
	vec2 UVs = TexCoords;
	if (flipX) UVs.x = 1.0 - UVs.x;
	if (flipY) UVs.y = 1.0 - UVs.y;

	vec4 colour = vec4(texture(sprite, UVs)); 
	if (useAlpha) colour.a = alpha;
	if (colour.a == 0.0)
		discard;

	if (colour.rgb == transparentColour)
		discard;

	fragColour = colour;
}
