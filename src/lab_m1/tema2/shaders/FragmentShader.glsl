#version 330

// Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 object_color;

uniform int hp;
uniform bool is_tank;

layout(location = 0) out vec4 out_color;

void main() {
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	float material_kd = 0.5f;
	float material_ks = 0.5f;
	int material_shininess = 1;

	float ambient_light = 0.8f;
	float diffuse_light = max(0.0, material_kd * max(dot(normalize(N), L), 0));
	float specular_light = 0.0f;

	if (diffuse_light > 0) {
		specular_light = max(0.0f, material_ks * pow(max(dot(normalize(N), H), 0), material_shininess));
	}

	// Compute light
	float light = ambient_light + diffuse_light + specular_light;

	vec3 color; 

	if(is_tank) {
		float percent = 1.0 - float(hp) / 3.0;
		color = mix(object_color, vec3(0.2, 0.2, 0.2), percent) * light;
	} else {
		color = object_color * light;
	}

	// Write pixel out color
	out_color = vec4(color, 1);
}
