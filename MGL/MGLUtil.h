#pragma once

/****** Defines ******/

// MGLShader

#define SHADER_VERTEX 0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2
#define SHADER_MAX 3
// MGLMesh

#define BUFFER_VERTEX 0
#define BUFFER_COLOUR 1
#define BUFFER_TEXTURE 2
#define BUFFER_INDICES 3
#define BUFFER_MAX 4

/****** Functions ******/

// Simple static functions that change state
// ENSURE MGL INIT WAS SUCCESSFUL!
namespace MGL {
	// Enabled wireframe
	static void EnableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	// Disables wireframe
	static void DisableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

}