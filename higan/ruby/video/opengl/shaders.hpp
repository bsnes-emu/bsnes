static string OpenGLOutputVertexShader = R"(
  #version 150

  uniform vec4 targetSize;
  uniform vec4 outputSize;

  in vec2 texCoord;

  out Vertex {
    vec4 position;
    vec2 texCoord;
  } vertexOut;

  void main() {
    //center image within output window
    if(gl_VertexID == 0 || gl_VertexID == 2) {
      gl_Position.x = -(targetSize.x / outputSize.x);
    } else {
      gl_Position.x = +(targetSize.x / outputSize.x);
    }

    //center and flip vertically (buffer[0, 0] = top-left; OpenGL[0, 0] = bottom-left)
    if(gl_VertexID == 0 || gl_VertexID == 1) {
      gl_Position.y = +(targetSize.y / outputSize.y);
    } else {
      gl_Position.y = -(targetSize.y / outputSize.y);
    }

    //align image to even pixel boundary to prevent aliasing
    vec2 align = fract((outputSize.xy + targetSize.xy) / 2.0) * 2.0;
    gl_Position.xy -= align / outputSize.xy;

    vertexOut.texCoord = texCoord;
  }
)";

static string OpenGLVertexShader = R"(
  #version 150

  in vec4 position;
  in vec2 texCoord;

  out Vertex {
    vec4 position;
    vec2 texCoord;
  } vertexOut;

  void main() {
    gl_Position = position;
    vertexOut.texCoord = texCoord;
  }
)";

static string OpenGLGeometryShader = R"(
  #version 150

  layout(triangles) in;
  layout(triangle_strip, max_vertices = 3) out;

  in Vertex {
    vec2 texCoord;
  } vertexIn[];

  out Vertex {
    vec2 texCoord;
  };

  void main() {
    for(int i = 0; i < gl_in.length(); i++) {
      gl_Position = gl_in[i].gl_Position;
      texCoord = vertexIn[i].texCoord;
      EmitVertex();
    }
    EndPrimitive();
  }
)";

static string OpenGLFragmentShader = R"(
  #version 150

  uniform sampler2D source[];

  in Vertex {
    vec2 texCoord;
  };

  out vec4 fragColor;

  void main() {
    fragColor = texture2D(source[0], texCoord);
  }
)";
