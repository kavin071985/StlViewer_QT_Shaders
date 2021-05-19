#shader vertex
#version 330 
layout (location = 0) in vec4 vertexPositionModel;	
layout (location = 1) in vec3 vertexColor;		   
layout (location = 2) in vec3 normalModel;		   //normal in model space
uniform mat4 u_modelToProjectionMatrix;			
uniform mat4 u_modelToWorldMatrix;			
out vec3 normalWorld;							  //normal in world space		
out vec4 vColor;									
out vec3 vertexPositionWorld;						
void main()                                        
{                                                  
	gl_Position = u_modelToProjectionMatrix*vertexPositionModel;		//MVP for the vertex positions
	normalWorld = vec3((u_modelToWorldMatrix)*vec4(normalModel,0));		//NVP for transforming normals to world coordinate space
	vertexPositionWorld =vec3(u_modelToWorldMatrix*vertexPositionModel); 
	vColor=vec4(vertexColor,1.0f) ;										//vertex color for fragment shader	
}

#shader fragment
#version 330                               
uniform vec3 u_lightPositionWorld;			
uniform vec4 u_ambientLight;					
in vec3 normalWorld;						
in vec4 vColor;							
in vec3 vertexPositionWorld;				
out vec4 colour;						   
void main()                               
{                                         
	vec3 lightVectorWorld=normalize(u_lightPositionWorld-vertexPositionWorld);		//calculates light unit vector for difuse light	
	float bright =dot(lightVectorWorld,normalize(normalWorld));
	vec4 diffuseLight = vec4(bright,bright,bright,1.0); 
	colour =vColor+clamp(diffuseLight,0,1)+u_ambientLight; 
}
