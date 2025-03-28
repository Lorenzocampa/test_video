#shader vertex
#version 120

attribute vec4 position;
attribute vec2 texCoord;
varying vec2 v_TexCoord;

void main()
{
    // Directly use the position attribute for gl_Position.
    gl_Position = position;
    v_TexCoord = texCoord;
}


// Fragment shader remains the sam
#shader fragment
#version 120

varying vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    // Use texture2D for texture lookup and use the varying variable.
    vec4 texColor = texture2D(u_Texture, v_TexCoord);
    gl_FragColor = texColor;
}


