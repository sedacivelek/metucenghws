#version 330

layout(location = 0) in vec3 position;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraposition;
uniform float heightFactor;
uniform vec3 lightPosition;
// Texture-related data
uniform sampler2D rHeightTexture;
uniform int widthTexture;
uniform int heightTexture;
uniform int offset;

// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
varying vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{

    // get texture value, compute height
    vec3 pos = position;
    float coordx = -(float(pos.x)+offset)/(widthTexture);
    float coordz = -float(pos.z)/(heightTexture);
    textureCoordinate = vec2(coordx,coordz);

    vec4 heightColor = texture2D(rHeightTexture,textureCoordinate);
    float height= heightFactor*heightColor.r;
    pos.y = height;

    vec3 westPos = pos-vec3(1,0,0);
    float westH = heightFactor*texture2D(rHeightTexture,vec2(-(westPos.x+offset)/widthTexture,1-westPos.z/heightTexture)).r;
    westPos.y = westH;

    vec3 eastPos = pos+vec3(1,0,0);
    float eastH = heightFactor*texture2D(rHeightTexture,vec2(-(eastPos.x+offset)/widthTexture,1-eastPos.z/heightTexture)).r;
    eastPos.y = eastH;

    vec3 northPos = pos-vec3(0,0,1);
    float northH = heightFactor*texture2D(rHeightTexture,vec2(-(northPos.x+offset)/widthTexture,1-northPos.z/heightTexture)).r;
    northPos.y = northH;

    vec3 southPos = pos+vec3(1,0,0);
    float southH = heightFactor*texture2D(rHeightTexture,vec2(-(southPos.x+offset)/widthTexture,1-southPos.z/heightTexture)).r;
    southPos.y = southH;

    vec3 neastPos = pos-vec3(-1,0,1);
    float neastH = heightFactor*texture2D(rHeightTexture,vec2(-(neastPos.x+offset)/widthTexture,1-neastPos.z/heightTexture)).r;
    neastPos.y = neastH;

    vec3 swestPos = pos+vec3(-1,0,1);
    float swestH = heightFactor*texture2D(rHeightTexture,vec2(-(swestPos.x+offset)/widthTexture,1-swestPos.z/heightTexture)).r;
    swestPos.y = swestH;
        
        
    vec3 norm1 = cross((southPos-pos),(eastPos-pos));
    vec3 norm2 = cross((swestPos-pos),(southPos-pos));
    vec3 norm3 = cross((westPos-pos),(swestPos-pos));
    vec3 norm4 = cross((northPos-pos),(westPos-pos));
    vec3 norm5 = cross((neastPos-pos),(northPos-pos));
    vec3 norm6 = cross((eastPos-pos),(neastPos-pos));
    vertexNormal = norm1+norm2+norm3+norm4+norm5+norm6;
    vertexNormal = normalize(vec3(vertexNormal.x,vertexNormal.y,vertexNormal.z)/6.0);
  
    // compute toLight vector vertex coordinate in VCS
    ToLightVector = normalize(lightPosition-pos);
    ToCameraVector = normalize(cameraposition.xyz - pos);
    gl_Position = MVP*vec4(position.x,height, position.z, 1.0);
    
}
