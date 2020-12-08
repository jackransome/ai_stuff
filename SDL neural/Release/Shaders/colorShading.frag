#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
in float fragmentLightAlpha;

uniform int lightArraySize;
uniform float lightColours[200*3];
uniform float lightPositions[200*2];
uniform float lightRadii[200];
uniform vec2 vignette; 
//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

uniform sampler2D mySampler;
uniform float ambientLight;
void main() {
	vec4 textureColor = texture(mySampler, fragmentUV);

	color = textureColor + fragmentColor;
	vec4 newLightColor = vec4(0.1, 0.1, 0.1, 1);
	float newLightIntensity = 0;
	for(int j = 0; j < lightArraySize; j++)
	{
		float d = distance(vec2(lightPositions[j*2], lightPositions[(j*2) + 1]), fragmentPosition);
		newLightIntensity = 1/(((d/lightRadii[j]))*((d/lightRadii[j])+1));
		if (fragmentLightAlpha > 0) {
			newLightColor += (vec4(lightColours[j*3], lightColours[(j*3) + 1], lightColours[(j*3) + 2], 1)*newLightIntensity)*fragmentLightAlpha;
		}
		else {
			newLightColor += (vec4(lightColours[j*3], lightColours[(j*3) + 1], lightColours[(j*3) + 2], 1)*newLightIntensity);
		}
	}
	if (fragmentLightAlpha <= 0) {
	//color = (1+fragmentLightAlpha)*color + ((-1*(fragmentLightAlpha)) * color * newLightColor);
	color = color + newLightColor/20;
	}
	else {
		color = color * newLightColor;
		color = color * ambientLight;
		color *= 2;
	}
	color.a = textureColor.a;
	if (fragmentColor.a > 0) {
	color.a = fragmentColor.a;
	}
	else
	{
	color.a = textureColor.a;
	}
	//ambient
	
	
	float dv = distance(fragmentPosition.xy, vignette);
	//vignette testing
	//color.a -= dv/1000;

}