//----- geometry shader -----
#version 330 core


layout(points) in;
layout(line_strip, max_vertices = 256) out;

uniform vec2 ScaledStep; // = Step / CameraPosition.z
uniform vec2 OriginPosition; // = ( -CameraPosition.x / (Viewport.width / 2),  -CameraPosition.y / (Viewport.height / 2) )

uniform int InvocationId;
//out vec4 color;
//-----------------------------------------------


void DrawGridYLines() 
{	
	float x0 = OriginPosition.x;

	/*// Y axis
	color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = vec4(x0, -1.0, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(x0, 1.0, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	color = Color;*/

	if( x0 > 1.0 )
		x0 -= (int(x0 / ScaledStep.x)) * ScaledStep.x;
	else if( x0 < -1.0 )
		x0 += (int(-x0 / ScaledStep.x)) * ScaledStep.x;
	
	// -X vertical lines (Y grid lines)
	float p = x0;
	while( p >= -1.0 )
	{
		gl_Position = vec4(p, -1.0, 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(p, 1.0, 0.0, 1.0);
		EmitVertex();
		EndPrimitive();
		p -= ScaledStep.x;
	}
	// +X vertical lines (Y grid lines)
	p = x0;
	while( p <= 1.0 )
	{
		gl_Position = vec4(p, -1.0, 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(p, 1.0, 0.0, 1.0);
		EmitVertex();
		EndPrimitive();
		p += ScaledStep.x;
	}
}

	
void DrawGridXLines()  
{	
	float y0 = OriginPosition.y;
	/*// X axis
	color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = vec4(-1.0, y0, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(1.0, y0, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	color = Color;*/

	if( y0 > 1.0 )
		y0 -= (int(y0 / ScaledStep.y)) * ScaledStep.y;
	else if( y0 < -1.0 )
		y0 += (int(-y0 / ScaledStep.y)) * ScaledStep.y;

	// -Y horizontal lines (X grid lines)
	float p = y0;
	while( p >= -1.0 )
	{
		gl_Position = vec4(-1.0, p, 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(1.0, p, 0.0, 1.0);
		EmitVertex();
		EndPrimitive();
		p -= ScaledStep.y;
	}
	// +Y horizontal lines (X grid lines)
	p = y0;
	while( p <= 1.0 )
	{
		gl_Position = vec4(-1.0, p, 0.0, 1.0);
		EmitVertex();
		gl_Position = vec4(1.0, p, 0.0, 1.0);
		EmitVertex();
		EndPrimitive();
		p += ScaledStep.y;
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
