//
// Created by Khoi Duc Minh Nguyen on 11/24/16.
//
#include <string>
using namespace std;
string vertex_shader = "#version 120\n"
        "\n"
        "uniform float\tuTime;\t\t// \"Time\", from Animate( )\n"
        "\n"
        "const float PI = \t3.14159265;\n"
        "const float AMP = \t0.2;\t\t// amplitude\n"
        "const float W = \t2.;\t\t// frequency\n"
        "\n"
        "varying vec2 vST;\n"
        "varying float angle;\n"
        "varying vec3 vN;\n"
        "varying vec3 vL;\n"
        "varying vec3 vE;\n"
        "\n"
        "void main( )\n"
        "{\n"
        "    vec3 LIGHTPOSITION = gl_LightSource[0].position.xyz;\n"
        "\n"
        "    vST = gl_MultiTexCoord0.st;\n"
        "\n"
        "    vec3 vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
        "    vec3 center = (gl_ModelViewMatrix * vec4(0, 0, 0, 1)).xyz;\n"
        "    vec3 center_to_light = normalize(LIGHTPOSITION - center);\n"
        "    vec3 center_to_vertex = normalize(vertex - center);\n"
        "    angle = acos(dot(center_to_light, center_to_vertex));\n"
        "\n"
        "    vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;\n"
        "    vN = normalize( gl_NormalMatrix * gl_Normal );\n"
        "    vL = LIGHTPOSITION - ECposition.xyz;\n"
        "    vE = vec3( 0., 0., 0. ) - ECposition.xyz;\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "\n"
        "}\0";

string fragment_shader = "#version 120\n"
        "\n"
        "uniform sampler2D texture_tex;\n"
        "uniform sampler2D earth_night_tex;\n"
        "uniform int planet;\n"
        "\n"
        "varying vec2 vST;\n"
        "varying float angle;\n"
        "varying vec3 vN;\n"
        "varying vec3 vL;\n"
        "varying vec3 vE;\n"
        "const float PI = \t3.14159265;\n"
        "\n"
        "\n"
        "void\n"
        "main( )\n"
        "{\n"
        "    vec3 uColor = vec3(1., 1., 1.);\n"
        "    vec3 uSpecularColor = vec3(1., 1., 1.);\n"
        "    float uShininess = 50.;\n"
        "    float uKa = .2;\n"
        "\n"
        "    if (planet == 1 || planet == 13 || planet == 14) { // Sun or Background or Ring\n"
        "        uKa = 1.;\n"
        "    }\n"
        "\n"
        "    float uKd = 1.;\n"
        "    float uKs = 1.;\n"
        "\n"
        "    vec3 Normal = normalize(vN);\n"
        "    vec3 Light = normalize(vL);\n"
        "    vec3 Eye = normalize(vE);\n"
        "\n"
        "    vec3 ambient = uKa * uColor;\n"
        "\n"
        "    float d = max( dot(Normal,Light), 0. );\n"
        "    vec3 diffuse = uKd * d * uColor;\n"
        "\n"
        "    float s = 0.;\n"
        "    if( dot(Normal,Light) > 0. ){\n"
        "        vec3 ref = normalize( reflect( -Light, Normal ) );\n"
        "        s = pow( max( dot(Eye,ref),0. ), uShininess );\n"
        "    }\n"
        "    vec3 specular = uKs * s * uSpecularColor;\n"
        "\n"
        "    vec3 out_color = vec3(1., 1., 1.);\n"
        "\n"
        "    if (planet == 4){\n"
        "        if (angle > PI/2){\n"
        "                out_color = (ambient + diffuse + specular) * texture2D( earth_night_tex, vST).rgb;\n"
        "        } else {\n"
        "            out_color = (ambient + diffuse + specular) * texture2D( texture_tex, vST).rgb;\n"
        "        }\n"
        "    } else {\n"
        "        out_color = (ambient + diffuse + specular) * texture2D( texture_tex, vST).rgb;\n"
        "    }\n"
        "\n"
        "    gl_FragColor = vec4( out_color, 1. );\n"
        "\n"
        "}\0";