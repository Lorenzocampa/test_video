
#shader vertex
#version 120
attribute vec2 position;
void main()
{
	// For a simple triangle, we simply build a vec4 from the attribute.
	gl_Position = vec4(position, 0.0, 1.0);
}

// Fragment shader remains the sam
#shader fragment
#version 120
void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

