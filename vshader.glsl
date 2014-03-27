#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

uniform float rollAngle;

attribute vec4 a_position;
attribute vec2 a_texcoord;
//attribute vec3 a_texcoord;
attribute vec3 a_normal;

varying vec2 v_texcoord;
//varying vec3 v_texcoord;
varying float v_dotLightFromSky;
varying float v_dotLightFromGround;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
 //--

//  vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));
  float ang = 2.0*rollAngle;///180.0*3.14159;
  vec3 lightFromSky = normalize(vec3(-sin(ang), cos(ang), 0.0));
  vec3 lightFromGround = normalize(vec3(-sin(ang), -cos(ang), 0.0));
//  vec3 toLight = normalize(vec3(0.0, 1.0, 0.0));

//  v_dotLightFromSky = max(dot(a_normal, lightFromSky), 0.0);
//  v_dotLightFromGround = max(dot(a_normal, lightFromGround), 0.0);
  v_dotLightFromSky = max(dot(a_normal, lightFromSky), 0.0);
  v_dotLightFromGround = max(dot(a_normal, lightFromGround), 0.0);

//  vec3 col = sourceColor.rgb;"
//  color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);"
//  color = clamp(color, 0.0, 1.0);"
}
//! [0]
