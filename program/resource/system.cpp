#include "main.h"




char* g_CacheBuffer = NULL;

GraphicsHelper g_GraphicsHelper;

GLuint g_VertexShaderId;
GLuint g_FragmentShaderId;
GLuint g_ShaderProgramId;




GLuint GetShaderProgramId()
{
	return g_ShaderProgramId;
}


const char* VertexShaderSource =
"#version 330\n"
"precision highp float;\n"

"uniform mat4 uProjection;\n"

"layout( location = 0 ) in vec3 inPosition;\n"
"layout( location = 1 ) in vec4 inColor;\n"
"layout( location = 2 ) in vec2 inTexCoord;\n"

"out vec4 vColor;\n"
"out vec2 vTexCoord;\n"

"void main() {\n"
"    vColor = inColor;\n"
"    vTexCoord = inTexCoord;\n"
"    gl_Position = vec4(inPosition, 1.0) * uProjection;\n"
"}\n";

const char* FragmentShaderSource =
"#version 330\n"
"precision highp float;\n"

"uniform sampler2D uSampler;\n"
"uniform bool uTextureEnable;\n"

"in vec4 vColor;\n"
"in vec2 vTexCoord;\n"

"out vec4 outColor;\n"

"void main() {\n"
"    if(uTextureEnable)\n"
"		outColor = vColor * texture(uSampler, vTexCoord);\n"
"    else\n"
"		outColor = vColor;\n"
"}\n";




void GL_APIENTRY DebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	NN_UNUSED(length);
	NN_UNUSED(userParam);

	NN_LOG("GL Debug Callback:\n");
	NN_LOG("  source:       0x%08x\n", source);
	NN_LOG("  type:         0x%08x\n", type);
	NN_LOG("  id:           0x%08x\n", id);
	NN_LOG("  severity:     0x%08x\n", severity);
	NN_LOG("  message:      %s\n", message);

	NN_ASSERT(type != GL_DEBUG_TYPE_ERROR, "GL Error occurs.");
}



void InitSystem()
{
	/*
		// ファイルシステムマウント
		{
			nn::Result result;
			size_t cacheSize = 0;

			nn::fs::QueryMountRomCacheSize(&cacheSize);

			g_CacheBuffer = new char[cacheSize];
			if (g_CacheBuffer == NULL)
			{
				NN_ASSERT(false, "Cache buffer is null.\n");
				return;
			}

			result = nn::fs::MountRom("rom", g_CacheBuffer, cacheSize);
			NN_ABORT_UNLESS_RESULT_SUCCESS(result);
		}
	*/


	// OpenGL初期化
	{
		g_GraphicsHelper.Initialize();

		NN_LOG("GL_VERSION: %s\n", glGetString(GL_VERSION));
		NN_LOG("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//glDebugMessageCallback(DebugCallbackFunc, NULL);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_DEPTH_TEST);

	}



	// シェーダ初期化
	{
		GLint result;
		GLchar shaderLog[1024];
		GLsizei shaderLogSize;

		g_VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		NN_ASSERT(g_VertexShaderId != 0, "Failed to create vertex shader\n");
		glShaderSource(g_VertexShaderId, 1, &VertexShaderSource, 0);
		glCompileShader(g_VertexShaderId);
		glGetShaderiv(g_VertexShaderId, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(g_VertexShaderId, sizeof(shaderLog), &shaderLogSize, shaderLog);
			NN_ASSERT(false, "Failed to compile vertex shader: %s\n", shaderLog);
		}

		g_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		NN_ASSERT(g_FragmentShaderId != 0, "Failed to create fragment shader\n");
		glShaderSource(g_FragmentShaderId, 1, &FragmentShaderSource, 0);
		glCompileShader(g_FragmentShaderId);
		glGetShaderiv(g_FragmentShaderId, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(g_FragmentShaderId, sizeof(shaderLog), &shaderLogSize, shaderLog);
			NN_ASSERT(false, "Failed to compile fragment shader: %s\n", shaderLog);
		}

		g_ShaderProgramId = glCreateProgram();
		NN_ASSERT(g_ShaderProgramId != 0, "Failed to create shader program\n");

		glAttachShader(g_ShaderProgramId, g_VertexShaderId);
		glAttachShader(g_ShaderProgramId, g_FragmentShaderId);
		glLinkProgram(g_ShaderProgramId);
		glUseProgram(g_ShaderProgramId);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		Matrix4x4f projection;
		projection = Matrix4x4f::OrthographicRightHanded(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f, 1.0f);

		Float4x4 fprojection;
		MatrixStore(&fprojection, projection);
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgramId, "uProjection"), 1, GL_TRUE, (float*)&fprojection);

	}

}




void UninitSystem()
{


	glDetachShader(g_ShaderProgramId, g_VertexShaderId);
	glDetachShader(g_ShaderProgramId, g_FragmentShaderId);

	glDeleteShader(g_VertexShaderId);
	glDeleteShader(g_FragmentShaderId);

	glDeleteProgram(g_ShaderProgramId);



	g_GraphicsHelper.Finalize();


	// ファイルシステムアンマウント
	{
		nn::fs::Unmount("rom");

		delete[] g_CacheBuffer;
		g_CacheBuffer = NULL;
	}


}


void SwapBuffers()
{
	g_GraphicsHelper.SwapBuffers();
}


