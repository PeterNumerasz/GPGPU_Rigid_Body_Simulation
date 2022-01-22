#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // v stands varying

uniform mat4 u_MVP; // MVP = Model view projection

void main() { 
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

// naming convention: variable names starting with "u_" are uniforms
// good to keep in mind
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() { 
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = texColor; 
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
