#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 128) out;

uniform mat4  ModelMatrix;
uniform mat4  ViewProjectionMatrix;
uniform float ScaledDistance; // distance(ViewPosition, ModelPosition) * gizmo scale
uniform int   GizmoType;
uniform int   InvocationId;

#define GIZMO_TYPE_TRANSLATE  1
#define GIZMO_TYPE_ROTATE     2
#define GIZMO_TYPE_SCALE      3
#define _2D_FLAG             16

flat out int ColorIndex;
flat out int Id;
//-----------------------------------------------

// A box with center at (0,0,0) and extent along each axis specified by r vector
void MakeBox(vec3 r, mat4 ViewProjectionMatrix) // r - radius
{ // number of emitted vertices: 18
	vec4 points_proj[8]; // for triangle strip, counter clockwise orientation
	points_proj[0] = ViewProjectionMatrix * vec4(-r.x, -r.y, r.z, 1.0); // left-bottom-near
	points_proj[1] = ViewProjectionMatrix * vec4(r.x, -r.y, r.z, 1.0); // right-bottom-near
	points_proj[2] = ViewProjectionMatrix * vec4(-r.x, r.y, r.z, 1.0); // left-top-near
	points_proj[3] = ViewProjectionMatrix * vec4(r.x, r.y, r.z, 1.0); // right-top-near
	points_proj[4] = ViewProjectionMatrix * vec4(-r.x, r.y, -r.z, 1.0); // left-top-far
	points_proj[5] = ViewProjectionMatrix * vec4(r.x, r.y, -r.z, 1.0); // right-top-far
	points_proj[6] = ViewProjectionMatrix * vec4(-r.x, -r.y, -r.z, 1.0); // left-bottom-far
	points_proj[7] = ViewProjectionMatrix * vec4(r.x, -r.y, -r.z, 1.0); // right-bottom-far

	// 6 faces: front - top - back
	for ( int i = 0; i < 8; ++i )
	{
		gl_Position = points_proj[i];
		EmitVertex();
	}
	// - bottom
	gl_Position = points_proj[0];
	EmitVertex();
	gl_Position = points_proj[1];
	EmitVertex();
	EndPrimitive();
	
	// left
	gl_Position = points_proj[6];
	EmitVertex();
	gl_Position = points_proj[0];
	EmitVertex();
	gl_Position = points_proj[4];
	EmitVertex();
	gl_Position = points_proj[2];
	EmitVertex();
	EndPrimitive();
	
	// right
	gl_Position = points_proj[1];
	EmitVertex();
	gl_Position = points_proj[7];
	EmitVertex();
	gl_Position = points_proj[3];
	EmitVertex();
	gl_Position = points_proj[5];
	EmitVertex();
	EndPrimitive();
}


// A cone with center at (0,0,0) and extended along Z axis
void MakeConeZ(vec2 r, vec2 e, int n, mat4 ViewProjectionMatrix) // r - radius(near, far), e - extent(near, far), n - granularity
{ // number of emitted vertices: 2*n + 2
	vec4 points[2];
	points[0] = vec4(0.0, r[1], -e[1], 1.0); // top-far
	points[1] = vec4(0.0, r[0], e[0], 1.0); // top-near
	
	float da = 6.28318531 / n;
	float ca = cos(da), sa = sin(da);
	vec4 p[2];
	p[0] = points[0];
	p[1] = points[1];
	mat4 RotateZMatrix = mat4(
	  ca, -sa,  0.0,  0.0,
	  sa,  ca,  0.0,  0.0,
	 0.0, 0.0,  1.0,  0.0,
	 0.0, 0.0,  0.0,  1.0);
	while( n-- != 0 ) {
		gl_Position = ViewProjectionMatrix * p[0];
		EmitVertex();
		gl_Position = ViewProjectionMatrix * p[1];
		EmitVertex();
		p[0] = RotateZMatrix * p[0];
		p[1] = RotateZMatrix * p[1];		
	}
	gl_Position = ViewProjectionMatrix * points[0];
	EmitVertex();
	gl_Position = ViewProjectionMatrix * points[1];
	EmitVertex();

	EndPrimitive();
}
//-----------------------------------------------

#define _X      1
#define _Y      2
#define _Z      3
#define _Center 0

#define AXIS_TIP_BOX   1
#define AXIS_TIP_ARROW 2

void DrawAxis(int axisId, int axisTip)
{
	mat4 GizmoMatrix;
	GizmoMatrix[0] = vec4(ScaledDistance, 0.0, 0.0, 0.0);
	GizmoMatrix[1] = vec4(0.0, ScaledDistance, 0.0, 0.0);
	GizmoMatrix[2] = vec4(0.0, 0.0, ScaledDistance, 0.0);
	GizmoMatrix[3] = ModelMatrix[3]; // (xyz position, 1.0)

#define axis_thikness 0.005
#define axis_tip_size 0.04
#define axis_tip_length 0.2 // for arrow only
#define axis_tip_granularity 8 // for arrow only
	float d = (axisId > 0 ? 0.5 : -0.5) * ScaledDistance;

	vec2 cone_r = vec2(0.0, axis_tip_size);
	
	switch ( axisId )
	{
		case _X:
		cone_r.xy = cone_r.yx;
		case -_X:
		GizmoMatrix[3].x += d;
		MakeBox( vec3(0.5, axis_thikness, axis_thikness), ViewProjectionMatrix * GizmoMatrix ); // x axis
		GizmoMatrix[3].x += d;
		switch ( axisTip ) // x axis tip
		{			
			case AXIS_TIP_BOX:
			MakeBox( vec3(axis_tip_size, axis_tip_size, axis_tip_size), ViewProjectionMatrix * GizmoMatrix );
			break;
			case AXIS_TIP_ARROW: {
			mat4 RotateY90Matrix = mat4(
			 0.0,  0.0,  1.0,  0.0,
			 0.0,  1.0,  0.0,  0.0,
			-1.0,  0.0,  0.0,  0.0,
			 0.0,  0.0,  0.0,  1.0);
			MakeConeZ(cone_r, vec2(0.0, axis_tip_length), axis_tip_granularity, ViewProjectionMatrix * GizmoMatrix * RotateY90Matrix);
			}
		}
		break;
	
		case -_Y:
		cone_r.xy = cone_r.yx;
		case _Y:
		GizmoMatrix[3].y += d;
		MakeBox( vec3(axis_thikness, 0.5, axis_thikness), ViewProjectionMatrix * GizmoMatrix ); // y axis
		GizmoMatrix[3].y += d;
		switch ( axisTip ) // y axis tip
		{
			case AXIS_TIP_BOX:		
			MakeBox( vec3(axis_tip_size, axis_tip_size, axis_tip_size), ViewProjectionMatrix * GizmoMatrix );
			break;
			case AXIS_TIP_ARROW: {
			const mat4 RotateX90Matrix = mat4(
			 1.0,  0.0,  0.0,  0.0,
			 0.0,  0.0, -1.0,  0.0,
			 0.0,  1.0,  0.0,  0.0,
			 0.0,  0.0,  0.0,  1.0);
			MakeConeZ(cone_r, vec2(0.0, axis_tip_length), axis_tip_granularity, ViewProjectionMatrix * GizmoMatrix * RotateX90Matrix);
			}
		}
		break;
	
		case -_Z:
		cone_r.xy = cone_r.yx;
		case _Z:
		GizmoMatrix[3].z += d;
		MakeBox( vec3(axis_thikness, axis_thikness, 0.5), ViewProjectionMatrix * GizmoMatrix ); // z axis
		GizmoMatrix[3].z += d;
		switch ( axisTip ) // z axis tip
		{
			case AXIS_TIP_BOX:		
			MakeBox( vec3(axis_tip_size, axis_tip_size, axis_tip_size), ViewProjectionMatrix * GizmoMatrix );
			break;
			case AXIS_TIP_ARROW:
			MakeConeZ(cone_r, vec2(0.0, axis_tip_length), axis_tip_granularity, ViewProjectionMatrix * GizmoMatrix);
		}
		break;

		case _Center:
		MakeBox( vec3(axis_tip_size, axis_tip_size, axis_tip_size), ViewProjectionMatrix * GizmoMatrix ); // center
	}
}


void DrawBand(int bandId)
{
	mat4 GizmoMatrix;
	GizmoMatrix[0] = vec4(normalize(ModelMatrix[0].xyz) * ScaledDistance, 0.0);
	GizmoMatrix[1] = vec4(normalize(ModelMatrix[1].xyz) * ScaledDistance, 0.0);
	GizmoMatrix[2] = vec4(normalize(ModelMatrix[2].xyz) * ScaledDistance, 0.0);
	GizmoMatrix[3] = ModelMatrix[3]; // (xyz position, 1.0)
	
	GizmoMatrix = ViewProjectionMatrix * GizmoMatrix;
	
	const float band_radius1 = 1.0,
	            band_radius2 = 0.9;
#define band_extent 0.0
#define band_granularity 32

	switch ( bandId )
	{
		case _X: {
		const mat4 RotateY90Matrix = mat4(
		 0.0,  0.0,  1.0,  0.0,
		 0.0,  1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,  0.0,
		 0.0,  0.0,  0.0,  1.0);
		MakeConeZ(vec2(band_radius1, band_radius2), vec2(band_extent, band_extent), band_granularity, GizmoMatrix * RotateY90Matrix);
		break; }

		case _Y: {
		const mat4 RotateX90Matrix = mat4(
		 1.0,  0.0,  0.0,  0.0,
		 0.0,  0.0,  1.0,  0.0,
		 0.0, -1.0,  0.0,  0.0,
		 0.0,  0.0,  0.0,  1.0);
		//band_radius1 *= 0.999; band_radius2 *= 0.999;
		MakeConeZ(vec2(band_radius1, band_radius2), vec2(band_extent, band_extent), band_granularity, GizmoMatrix * RotateX90Matrix);
		break; }

		case _Z:
		//band_radius1 *= 0.999; band_radius2 *= 0.999;
		MakeConeZ(vec2(band_radius2, band_radius1), vec2(band_extent, band_extent), band_granularity, GizmoMatrix);
	}
}
//-----------------------------------------------


void main() 
{	
	ColorIndex = InvocationId;
	Id = InvocationId;
	if ( GizmoType > _2D_FLAG ) // 2D view: XY plane
	{
		switch ( GizmoType )
		{
			case GIZMO_TYPE_TRANSLATE + _2D_FLAG: // draw X and Y axes only
			if ( InvocationId == 0 )
				DrawAxis(_X, AXIS_TIP_ARROW);
			else if ( InvocationId == 1 ) {
				ColorIndex = 2; // Blue
				DrawAxis(-_Y, AXIS_TIP_ARROW); // inverted Y axis - blue ( = Z axis in 3D view)
			}
			break;
			
			case GIZMO_TYPE_SCALE + _2D_FLAG: // draw X and Y axes only
			if ( InvocationId == 0 )
				DrawAxis(_X, AXIS_TIP_BOX);
			else if ( InvocationId == 1 )
			{
				ColorIndex = 2; // Blue
				DrawAxis(-_Y, AXIS_TIP_BOX); // inverted Y axis - blue ( = Z axis in 3D view)
				// ad hoc to draw central box:
				ColorIndex = 3;
				Id = 3;
				DrawAxis(_Center, 0);
			}	
			break;

			case GIZMO_TYPE_ROTATE + _2D_FLAG:
			if ( InvocationId == 2 ) {
				ColorIndex = 1; // Green
				DrawBand(_Z); // Z band - green ( = Y band in 3D view)
			}
		}
	}
	
	else // 3D view
	{
		switch ( GizmoType )
		{
			case GIZMO_TYPE_TRANSLATE : // draw all 3 axes with arrow tips
			if ( InvocationId < 3 )
				DrawAxis(_X + InvocationId, AXIS_TIP_ARROW);
			break;
			
			case GIZMO_TYPE_SCALE : // draw all 3 axes with box tips, draw central box
			if ( InvocationId < 3 )
			{
				DrawAxis(_X + InvocationId, AXIS_TIP_BOX);
				// ad hoc to draw central box:
				if( InvocationId == 2 )
				{
					ColorIndex = 3;
					Id = 3;
					DrawAxis(_Center, 0);
				}
			}
			break;

			case GIZMO_TYPE_ROTATE :
			if ( InvocationId == 1 )
				DrawBand(_Y); // Y band - green
		}
	}
}
