#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform int displacementFlag;
uniform float time;

// Output value to fragment shader
out vec3 color;

float rand(vec3 position)
{
    return fract(35 * (position.x + position.y + position.z) / 3.0);
}

void main()
{
    // Send color light output to fragment shader
    color = vec3(1);
    color = object_color;

    float randNr = rand(v_position);
    vec3 displacement = v_position + v_position * randNr * abs(sin(time));

    if (displacementFlag == 1)
    {
        gl_Position = Projection * View * Model * vec4(displacement, 1.0);
    }
    else
    {
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    }
}
