#version 120

uniform float	uTime;		// "Time", from Animate( )
uniform sampler2D texture_tex;
uniform sampler2D earth_night_tex;
uniform int is_sun;

varying vec2 vST;
varying float angle;
varying vec3 vN;
varying vec3 vL;
varying vec3 vE;
varying float d;
varying float d2;
varying float d3;
const float PI = 	3.14159265;


void
main( )
{
    vec3 uColor = vec3(1., 1., 1.);
    vec3 uSpecularColor = vec3(1., 1., 1.);
    float uShininess = 50.;
    float uKa = .1;

    if (angle > PI/2){
        uKa = .2;
    }

    if (d < 1 || d2 > 100. || (d3 >=0.25 && d3 <= 0.4)){
        uKa = 1.;
    }

    float uKd = 1.;
    float uKs = 1.;

    vec3 Normal = normalize(vN);
    vec3 Light = normalize(vL);
    vec3 Eye = normalize(vE);

    vec3 ambient = uKa * uColor;

    float d = max( dot(Normal,Light), 0. );
    vec3 diffuse = uKd * d * uColor;

    float s = 0.;
    if( dot(Normal,Light) > 0. ){
        vec3 ref = normalize( reflect( -Light, Normal ) );
        s = pow( max( dot(Eye,ref),0. ), uShininess );
    }
    vec3 specular = uKs * s * uSpecularColor;

    vec3 out_color = vec3(1., 1., 1.);

    if (angle > PI/2){
        out_color = (ambient + diffuse + specular) * texture2D( earth_night_tex, vST).rgb;
    } else {
        out_color = (ambient + diffuse + specular) * texture2D( texture_tex, vST).rgb;
    }

    gl_FragColor = vec4( out_color, 1. );

}
