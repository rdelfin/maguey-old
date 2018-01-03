//
// Created by rdelfin on 1/3/18.
//
// This file contains some standard shaders used as defaults in the meshes
// or as the shaders for special objects like the skybox.

#include <string>

const std::string mesh_shader_vert =
R"zzz(
#version 400 core

uniform vec3 camera_position;
uniform vec4 light_position = vec4(0.0, 100.0, 20.0, 1.0);

in vec4 vertex_position;
in vec4 normal;


out vec4 vs_normal;
out vec4 vs_camera_direction;
out vec4 vs_light_direction;

void main() {
    gl_Position = vertex_position;
    vs_normal = normal;
    vs_camera_direction = normalize(vec4(camera_position - vec3(gl_Position), 0.0));
    vs_light_direction = normalize(light_position - gl_Position);
}
)zzz";

const std::string mesh_shader_geom =
R"zzz(
#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in vec4 vs_camera_direction[];
in vec4 vs_normal[];
in vec4 vs_light_direction[];

out vec4 camera_direction;
out vec4 vertex_normal;
out vec4 world_position;
out vec4 light_direction;

void main() {
    int n = 0;

    for(n = 0; n < gl_in.length(); n++) {
        camera_direction = vs_camera_direction[n];
        light_direction = vs_light_direction[n];
        vertex_normal = model * vs_normal[n];
        gl_Position = projection * view * model * gl_in[n].gl_Position;
        world_position = model * gl_in[n].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
)zzz";

const std::string mesh_shader_frag =
R"zzz(
#version 400 core
#line 1

in vec4 vertex_normal;
in vec4 camera_direction;
in vec4 world_position;
in vec4 light_direction;

uniform vec3 texcolor = vec3(1.0, 1.0, 1.0);
uniform vec3 diffuse = vec3(0.50, 0.50, 0.50);
uniform vec3 ambient = vec3(0.14, 0.14, 0.14);
uniform vec3 specular = vec3(1.00, 1.00, 1.00);
uniform float alpha = 1.0;
uniform float shininess = 1.0;

out vec4 vertex_color;

void main() {
    float dot_nl = dot(light_direction, normalize(vertex_normal));
    dot_nl = clamp(dot_nl, 0.0, 1.0);
    vec3 spec = specular * max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction));
    vec3 color = clamp(ambient + dot_nl*diffuse/* + spec*/, 0.0, 1.0);
    vertex_color = vec4(color, alpha);
}
)zzz";


std::string skybox_shader_vert =
R"zzz(
#version 400 core

in vec4 vertex_position;

out vec3 vertex_tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vertex_tex_coords = vec3(vertex_position);
    gl_Position = projection * view * vertex_position;
}
)zzz";

std::string skybox_shader_geom =
R"zzz(
#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vertex_tex_coords[];

out vec3 TexCoords;

void main() {
    int n = 0;

    for(n = 0; n < gl_in.length(); n++) {
        gl_Position = gl_in[n].gl_Position;
        TexCoords = vertex_tex_coords[n];
        EmitVertex();
    }

    EndPrimitive();
}
)zzz";

std::string skybox_shader_frag =  R"zzz(
#version 400 core
out vec4 vertex_color;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    vertex_color = texture(skybox, TexCoords);
}
)zzz"