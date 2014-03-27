#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
//varying vec3 v_texcoord;
varying float v_dotLightFromSky;
varying float v_dotLightFromGround;

//! [0]
void main()
{
    // Set fragment color from texture
//-    gl_FragColor = texture2D(texture, v_texcoord);
//--

//  vec3 col = sourceColor.rgb;"
//  color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);"
//  color = clamp(color, 0.0, 1.0);"

  vec4 srcPixel = texture2D(texture, v_texcoord);
//  vec4 color  = vec4(srcPixel * 0.2 + srcPixel * 0.8 * v_cosangle)*4.0;//, 1.0);
  vec4 color  = vec4(srcPixel*0.2 + srcPixel*(0.8*v_dotLightFromSky + 0.4*v_dotLightFromGround))*3.0;//, 1.0);
  color.w = 1.0;

//  lowp vec4 yuvPixel = rgbToYuv * srcPixel;
//  yuvPixel.r *= 3.0;
//  gl_FragColor = yuvToRgb * yuvPixel;

  gl_FragColor = clamp(color, 0.0, 1.0);
//gl_FragColor = texture2D(texture, v_texcoord);

}
//! [0]

