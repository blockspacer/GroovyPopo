
#include "main.h"

unsigned int LoadTexture(const char *FileName)
{
/*
	nn::Result result;
	nn::fs::FileHandle file;
	size_t readSize;

	result = nn::fs::OpenFile(&file, FileName, nn::fs::OpenMode_Read);
	if (nn::fs::ResultPathNotFound::Includes(result))
	{
		NN_ASSERT(false, "OpenFile:", FileName);
	}
*/
	FILE* file;
	file = fopen(FileName, "rb");
	if (file == NULL)
	{
		return -1;
	}

	unsigned int	texture;
	unsigned char	header[18];
	unsigned char	*image;
	unsigned int	width, height;
	unsigned char	depth;
	unsigned int	bpp;
	unsigned int	format;


	// ヘッダ読み込み
	//nn::fs::ReadFile(&readSize, file, 0, header, sizeof(header));
	fread(header, sizeof(header), 1, file);

	// 画像サイズ取得
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
	{
		bpp = 4;
		format = GL_RGBA;
	}
	else if (depth == 24)
	{
		bpp = 3;
		format = GL_RGB;
	}
	else
	{
		bpp = 0;
		NN_ASSERT( false, "LoadTexture:", FileName );
	}


	// メモリ確保
	image = (unsigned char*)new unsigned char[width * height * bpp];

	// 画像読み込み
	//nn::fs::ReadFile(&readSize, file, sizeof(header), image, width * height * bpp);
	fread(image, width * height * bpp, 1, file);

	// R<->B
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned char c;
			c = image[(y * width + x) * bpp + 0];
			image[(y * width + x) * bpp + 0] = image[(y * width + x) * bpp + 2];
			image[(y * width + x) * bpp + 2] = c;
		}
	}

	//nn::fs::CloseFile(file);
	fclose(file);


	// テクスチャ生成
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	// ミップマップ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, 0);

	delete image;

	return texture;
}

void UnloadTexture(unsigned int Texture)
{

	glDeleteTextures(1, &Texture);

}

void SetTexture(unsigned int Texture)
{
	if (Texture == 0)
	{
		glUniform1i(glGetUniformLocation(GetShaderProgramId(), "uTextureEnable"), 0);
	}
	else
	{
		glUniform1i(glGetUniformLocation(GetShaderProgramId(), "uTextureEnable"), 1);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
}


