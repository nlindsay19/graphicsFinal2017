#version 330 core

out vec4 FragColor;
out vec4 BrightColor;

in vec3 TexCoords;
in vec4 pos;

in vec3 inNormal;

uniform samplerCube sun;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[6];
uniform vec3 viewPos;

void main()
{
   vec4 color = texture(sun, TexCoords);

   vec3 normal = normalize(inNormal);

   //ambient
   vec3 ambient = vec3(0.f);
   vec3 lighting = vec3(0.0);

   for (int i = 0; i < 6; i++) {
       // diffuse
       vec3 lightDir = normalize(lights[i].position - pos.xyz);
       float diff = max(dot(lightDir, normal), 0.f);
       //obj.material.blend * glm::vec4(iRed, iGreen, iBlue, 255) + (1.f - obj.material.blend) * objectDiff
       vec3 diffuse = 0.01f*lights[i].color * diff * color.xyz;//0.2f * color.xyz + (1.f - 0.2f)  * diff;//lights[i].color * diff * color.xyz;
       vec3 result = diffuse;
       lighting += result;
   }
   vec3 result = lighting + ambient;
   float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
   if (brightness > 1.0) {
       BrightColor = vec4(result, 1.0);
   } else {
       BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
   }
   FragColor = vec4(result, 1.0);

}
