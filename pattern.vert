#version 120

uniform float	uTime;		// "Time", from Animate( )

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency

varying vec2 vST;
varying float angle;
varying vec3 vN;
varying vec3 vL;
varying vec3 vE;
varying float d;
varying float d2;
varying float d3;

void main( )
{
    vec3 LIGHTPOSITION = gl_LightSource[0].position.xyz;

    vST = gl_MultiTexCoord0.st;

    vec3 vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vec3 center = (gl_ModelViewMatrix * vec4(0, 0, 0, 1)).xyz;
    vec3 center_to_light = normalize(LIGHTPOSITION - center);
    vec3 center_to_vertex = normalize(vertex - center);
    angle = acos(dot(center_to_light, center_to_vertex));
    d = distance(LIGHTPOSITION, center);
    d2 = distance(vertex, center);
    d3 = distance(gl_Vertex.xyz, vec3(0,0,0));

    vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
    vN = normalize( gl_NormalMatrix * gl_Normal );
    vL = LIGHTPOSITION - ECposition.xyz;
    vE = vec3( 0., 0., 0. ) - ECposition.xyz;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}
