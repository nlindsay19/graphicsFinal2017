#version 330 core

out vec4 FragColor;
out vec4 BrightColor;

in vec3 TexCoords;
in vec4 pos;
in vec2 normalTexCoords;

in vec3 inNormal;
in mat3 TBN;
in mat3 MV3x3;
in mat4 V;
in mat4 M;

uniform samplerCube planet;
uniform sampler2D normals;
uniform int ifBump;
uniform int bezier;
uniform int useLighting;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[5];
uniform vec3 viewPos;

void main()
{
    vec4 color = texture(planet, TexCoords);

    vec3 normal = normalize(inNormal);

    if (ifBump == 1) {
        normal = normalize(texture(normals, normalTexCoords).rgb * 2.f - 1.f); // in tangent space
    }

    //ambient
    vec3 ambient = vec3(0.f);
    vec3 lighting = vec3(0.f);

    for (int i = 0; i < 4; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - pos.xyz);
        if (ifBump == 1) {
            lightDir = normalize(TBN * (V * vec4((lights[i].position - pos.xyz), 1.f)).xyz); // tangent space
        }

        float diff = clamp(dot(lightDir, normal), 0.f, 1.f);
        // Ex: obj.material.blend * glm::vec4(iRed, iGreen, iBlue, 255) + (1.f - obj.material.blend) * objectDiff
        vec3 diffuse = diff * color.xyz;
        vec3 result = diffuse;

        // attenuation
        float distance = length(pos.xyz - lights[i].position);
        result *= 1.f / 3.f;
        lighting += result;
    }
    // neptune lighting
    if (bezier == 1) {
        // diffuse
        vec3 lightDir = normalize(lights[4].position - pos.xyz);
        float diff = clamp(dot(lightDir, normal), 0.f, 1.f);
        vec3 diffuse = diff * color.xyz;
        vec3 result = diffuse;
        // attenuation
        float distance = length(pos.xyz - lights[4].position);
        result *= 1.f / 3.f;
        lighting += result;
    }

    vec4 r = vec4(ambient + lighting, 255.f);
    float brightness = dot(r.xyz, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
        BrightColor = r;
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    if (useLighting == 1) {
        FragColor = r;
    } else {
        FragColor = color;
    }
}


