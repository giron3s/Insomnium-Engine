#version 330 core

#define NUM_CIRCLE_VERTICES 40

layout(points) in; // not used
layout(line_strip, max_vertices=NUM_CIRCLE_VERTICES) out;

#define BOX 1
#define SPHERE 2

uniform int  u_type; // {BOX, SPHERE}
uniform vec3 u_vertex0; // min for box, center for sphere
uniform vec3 u_vertex1; // max for box, .x is radius for sphere (.y, .z are not used for sphere)

uniform mat4 u_MVPMatrix;
uniform mat4 u_ProjectionMatrix; // needed only for sphere



void RenderBox()
{
	vec4 v_left_bottom_near = u_MVPMatrix * vec4(u_vertex0, 1.0),
	     v_right_bottom_near = u_MVPMatrix * vec4(u_vertex1.x, u_vertex0.y, u_vertex0.z, 1.0),
		 v_right_top_near = u_MVPMatrix * vec4(u_vertex1.x, u_vertex1.y, u_vertex0.z, 1.0),
		 v_left_top_near = u_MVPMatrix * vec4(u_vertex0.x, u_vertex1.y, u_vertex0.z, 1.0),
		 v_left_bottom_far = u_MVPMatrix * vec4(u_vertex0.x, u_vertex0.y, u_vertex1.z, 1.0),
	     v_right_bottom_far = u_MVPMatrix * vec4(u_vertex1.x, u_vertex0.y, u_vertex1.z, 1.0),
		 v_right_top_far = u_MVPMatrix * vec4(u_vertex1, 1.0),
		 v_left_top_far = u_MVPMatrix * vec4(u_vertex0.x, u_vertex1.y, u_vertex1.z, 1.0);
		 
	// main contour: near face, left bootom line, far face
	gl_Position = v_left_bottom_near;
    EmitVertex();
    gl_Position = v_right_bottom_near;
    EmitVertex();
    gl_Position = v_right_top_near;
    EmitVertex();
    gl_Position = v_left_top_near;
    EmitVertex();
    gl_Position = v_left_bottom_near;
    EmitVertex();
    gl_Position = v_left_bottom_far;
    EmitVertex();
    gl_Position = v_right_bottom_far;
    EmitVertex();
    gl_Position = v_right_top_far;
    EmitVertex();
    gl_Position = v_left_top_far;
    EmitVertex();
    gl_Position = v_left_bottom_far;
    EmitVertex();
    EndPrimitive();
	
	// left top line
    gl_Position = v_left_top_near;
    EmitVertex();
    gl_Position = v_left_top_far;
    EmitVertex();
    EndPrimitive();
	
	// right top line
    gl_Position = v_right_top_near;
    EmitVertex();
    gl_Position = v_right_top_far;
    EmitVertex();
    EndPrimitive();
	
	// right bottom line
    gl_Position = v_right_bottom_near;
    EmitVertex();
    gl_Position = v_right_bottom_far;
    EmitVertex();
    EndPrimitive();
}


void RenderSphere()
{
    vec4 v_center = u_MVPMatrix * vec4(u_vertex0, 1.0); // center in projection space
	float r = u_vertex1.x; // radius value assumed to be in projection space already!
	const float M_2PI = 3.141593 * 2.0;
    for ( int i = 0; i < NUM_CIRCLE_VERTICES; ++i )
    { // Generate a circle around the center of NUM_CIRCLE_VERTICES vertices
		float a = i*M_2PI/(NUM_CIRCLE_VERTICES-1);
		vec4 v = u_ProjectionMatrix * vec4(r * cos(a), r * sin(a), 0.0, 1.0); // a point on the circle in projection space
        gl_Position = vec4(v_center.x + v.x, v_center.y + v.y, v_center.z, v_center.w);
        EmitVertex();
    }
    EndPrimitive();
}


void main()
{
	switch ( u_type )
	{
		case BOX: RenderBox(); break;
		case SPHERE: RenderSphere();
	}
}
