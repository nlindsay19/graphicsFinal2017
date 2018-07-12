#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in vec2 aTexCoords;

out vec3 TexCoords;

out vec3 pos;
out vec3 inNormal;
out mat3 TBN;
out vec2 normalTexCoords;
out mat3 MV3x3;
out mat4 V;
out mat4 M;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
    pos = (model * vec4(aPos, 1.f)).xyz; // worldspace
    normalTexCoords = aTexCoords;
    V = view;
    M = model;

    // Matrix to transform normal from object to world space
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    inNormal = normalize(normalMatrix * aNorm); // worldspace

    MV3x3 = mat3(view * model);
    vec3 vertexNormal_cameraspace = MV3x3 * normalize(aNorm);
    vec3 vertexTangent_cameraspace = MV3x3 * normalize(aTangent);
    vec3 vertexBitangent_cameraspace = MV3x3 * normalize(aBiTangent);

    TBN = transpose(mat3(vertexTangent_cameraspace, vertexBitangent_cameraspace, vertexNormal_cameraspace)); // in camera space

    TexCoords = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0); // clip space
}
