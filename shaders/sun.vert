#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 TexCoords;

out vec3 pos;
out vec3 inNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
    pos = vec3(model * vec4(aPos, 1.f));

    vec3 n = aNorm;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    inNormal = normalize(normalMatrix * n);
    TexCoords = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}
