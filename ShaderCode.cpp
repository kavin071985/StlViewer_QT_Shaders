
 const char* vShader1 = "															\n\
 #version 330                                        \n\
												   \n\
 layout (location = 0) in vec4 vertexPositionModel;		   \n\
 layout (location = 1) in vec3 vertexColor;		   \n\
 layout (location = 2) in vec3 normalModel;		   \n\
 uniform mat4 modelToProjectionMatrix;			\n\
 uniform mat4 modelToWorldMatrix;			\n\
 out vec3 normalWorld;												\n\
 out vec4 vColor;												\n\
 out vec3 vertexPositionWorld;												\n\
 void main()                                         \n\
 {                                                   \n\
		gl_Position = modelToProjectionMatrix*vertexPositionModel; \n\
		normalWorld = vec3((modelToWorldMatrix)*vec4(normalModel,0)); \n\
		vertexPositionWorld =vec3(modelToWorldMatrix*vertexPositionModel); \n\
		vColor=vec4(vertexColor,1.0f) ;												\n\
 }";

const char* fShader1 = "				     	\n\
 #version 330                                    \n\
 uniform vec3 lightPositionWorld;											\n\
 uniform vec4 ambientLight;											\n\
 in vec3 normalWorld;												\n\
 in vec4 vColor;												\n\
 in vec3 vertexPositionWorld;												\n\
 out vec4 colour;						     \n\
 void main()                                        \n\
 {                                                   \n\
 vec3 lightVectorWorld=normalize(lightPositionWorld-vertexPositionWorld);			\n\
 float bright =dot(lightVectorWorld,normalize(normalWorld));\n\
 vec4 diffuseLight = vec4(bright,bright,bright,1.0); \n\
 colour =vColor+clamp(diffuseLight,0,1)+ambientLight;  \n\
 }";