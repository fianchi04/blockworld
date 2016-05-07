#version 130

in vec3 position;
in vec3 inputcolor;

out vec3 frag_color;

//added 8/12
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
//added 05/16
uniform mat4 Anim;



void main() {

	//added 8/12
	gl_Position = (Projection * View * Model * Anim) * vec4(position, 1);
        frag_color = inputcolor;
}
