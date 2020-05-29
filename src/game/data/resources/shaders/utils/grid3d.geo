//----- geometry shader -----
#version 330 core


layout(points) in;
layout(line_strip, max_vertices = 256) out;

uniform float ScaledStep;
uniform float GridSize;
uniform mat4  ViewProjectionMatrix;

uniform int InvocationId;

//-----------------------------------------------


void DrawGridYLines() 
{	
	// -X vertical lines (Y grid lines)
	float p = 0;
	while( p >= -GridSize )
	{
		gl_Position = ViewProjectionMatrix * vec4(p, 0.0, -GridSize, 1.0);
		EmitVertex();
		gl_Position = ViewProjectionMatrix * vec4(p, 0.0, GridSize, 1.0);
		EmitVertex();
		EndPrimitive();
		p -= ScaledStep;
	}
	
	// +X vertical lines (Y grid lines)
	p = 0;
	while( p <= GridSize )
	{
		gl_Position = ViewProjectionMatrix * vec4(p, 0.0, -GridSize, 1.0);
		EmitVertex();
		gl_Position = ViewProjectionMatrix * vec4(p, 0.0, GridSize, 1.0);
		EmitVertex();
		EndPrimitive();
		p += ScaledStep;
	}
}

	
void DrawGridXLines() 
{	
	// -Y horizontal lines (X grid lines)
	float p = 0;
	while( p >= -GridSize )
	{
		gl_Position = ViewProjectionMatrix * vec4(-GridSize, 0.0, p, 1.0);
		EmitVertex();
		gl_Position = ViewProjectionMatrix * vec4(GridSize, 0.0, p, 1.0);
		EmitVertex();
		EndPrimitive();
		p -= ScaledStep;
	}
	
	// +Y horizontal lines (X grid lines)
	p = 0;
	while( p <= GridSize )
	{
		gl_Position = ViewProjectionMatrix * vec4(-GridSize, 0.0, p, 1.0);
		EmitVertex();
		gl_Position = ViewProjectionMatrix * vec4(GridSize, 0.0, p, 1.0);
		EmitVertex();
		EndPrimitive();
		p += ScaledStep;
	}
}


void main()
{
	switch( InvocationId )
	{
		case 0: DrawGridXLines(); break;
		case 1: DrawGridYLines(); 
	}
}
//---------------------------
