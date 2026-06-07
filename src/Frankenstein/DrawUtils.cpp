/***********************************************************************
Utilities for loading and drawing sprites.
*/
#include"DrawUtils.h"
#include<GL/glew.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

using namespace DrawUtilities;

/* Load a file into an OpenGL texture, and return that texture. */
GLuint DrawUtilities::glTexImageTGAFile(const char* filename)
{
	const int BPP = 4;

	/* open the file */
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "File: %s -- Could not open for reading.\n", filename);
		return 0;
	}

	/* skip first two bytes of data we don't need */
	fseek(file, 2, SEEK_CUR);

	/* read in the image type.  For our purposes the image type should
	* be either a 2 or a 3. */
	unsigned char imageTypeCode;
	fread(&imageTypeCode, 1, 1, file);
	if (imageTypeCode != 2 && imageTypeCode != 3)
	{
		fclose(file);
		fprintf(stderr, "File: %s -- Unsupported TGA type: %d\n", filename, imageTypeCode);
		return 0;
	}

	/* skip 9 bytes of data we don't need */
	fseek(file, 9, SEEK_CUR);

	/* read image dimensions */
	int imageWidth = 0;
	int imageHeight = 0;
	int bitCount = 0;
	fread(&imageWidth, sizeof(short), 1, file);
	fread(&imageHeight, sizeof(short), 1, file);
	fread(&bitCount, sizeof(unsigned char), 1, file);
	fseek(file, 1, SEEK_CUR);

	/* sanity-check dimensions before allocating */
	if (imageWidth <= 0 || imageHeight <= 0)
	{
		fclose(file);
		fprintf(stderr, "File: %s -- Invalid TGA dimensions: %dx%d\n", filename, imageWidth, imageHeight);
		return 0;
	}

	/* allocate memory for image data and read it in */
	unsigned char* bytes = (unsigned char*)calloc(imageWidth * imageHeight * BPP, 1);
	if (bytes == NULL)
	{
		fclose(file);
		fprintf(stderr, "File: %s -- Could not allocate %d bytes for image data.\n", filename, imageWidth * imageHeight * BPP);
		return 0;
	}

	/* read in data */
	if (bitCount == 32)
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = fgetc(file);
		}
	}
	else
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = 255;
		}
	}

	fclose(file);

	/* load into OpenGL */
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, bytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(bytes);

	return tex;
}

/* Draw the sprite using default values for s and t texture coordinates
*  Assumes the full texture will be drawn
 * The lower left of a texture is (0.0, 0.0) and upper right is (1.0, 1.0)
 * Horizontal axis is s, and vertical axis is t
 * param tex is an openGL texture mapped to a quad
 * param x is position to draw sprite in the game world
 * param y is position to draw sprite in the game world
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn*/
void DrawUtilities::glDrawSprite(GLuint tex, int x, int y, int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(1.0f, 1.0f); // top right
		glVertex2i(x + w, y);

		glTexCoord2f(1.0f, 0.0f); // bottom right
		glVertex2i(x + w, y + h);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + h);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}

// Draws a single frame of a GL texture
void DrawUtilities::glDrawFrame(GlDrawFrameParams params)
{
	glBindTexture(GL_TEXTURE_2D, params.tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(params.s1, params.t2);  // A
		glVertex2i(params.x, params.y);

		glTexCoord2f(params.s2, params.t2);  // B
		glVertex2i(params.x + params.w, params.y);

		glTexCoord2f(params.s2, params.t1); // C
		glVertex2i(params.x + params.w, params.y + params.h);

		glTexCoord2f(params.s1, params.t1); // D
		glVertex2i(params.x, params.y + params.h);
	}
	glEnd();
}


/* drawRasterText - Draws a text string at (x, y) in the game world. tex must be an font image
 * param tex - the font image
 * param x - x position in the game world to draw the string
 * param y - y position in the game world to draw the string
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn
 */
void DrawUtilities::drawRasterText(GLuint tex, int x, int y, int w, int h, char string[])
{
	float s1 = 0.0f; //---------------define just to get it to compile, fix later
	float s2 = 0.0f; //---------------define just to get it to compile, fix later
	float t1 = 0.0f; //---------------define just to get it to compile, fix later
	float t2 = 0.0f; //---------------define just to get it to compile, fix later
	float frameDivision = 0.0f; //---------------define just to get it to compile, fix later
	float rowDivision = 0.0f; //---------------define just to get it to compile, fix later
	int numberOfFrames = 0; //-----------------define just to get it to compile, fix later
	int numberOfRows = 0; //-----------------define just to get it to compile, fix later
	int currentRow = 0; //-----------------define just to get it to compile, fix later

	// NOTE: This helper is still a stub (the font metrics above are placeholders);
	// the working font-drawing path is TextString::drawText. Guard against the
	// divide-by-zero that the zero-initialized numberOfFrames would otherwise cause.
	if (numberOfFrames <= 0)
		return;

	for (int i = 0; i < strlen(string); i++)
	{
		int frame = string[i] - 32; //the current frame or letter to draw
		// Keep currentFrame in range
		if (frame >= numberOfFrames)
			frame = frame % numberOfFrames;

		// current row may change. Use modulus
		if (s2 >= 1.0 && numberOfRows > 1)
			currentRow = (currentRow + 1) % numberOfRows;

		// update s1, s2, t1, t2
		s1 = frame * frameDivision;
		s2 = (frame * frameDivision) + frameDivision;
		t1 = currentRow * rowDivision;
		t2 = (currentRow * rowDivision) + rowDivision;

		GlDrawFrameParams params;
		params.tex = tex;
		params.x = x;
		params.y = y;
		params.w = w;
		params.h = h;
		params.s1 = s1;
		params.s2 = s2;
		params.t1 = t1;
		params.t2 = t2;

		glDrawFrame(params);
	}
}

// Draws a dim world-space reference grid so camera scrolling is visible.
void DrawUtilities::drawGrid(int worldWidth, int worldHeight, int cell)
{
	if (cell <= 0)
		return;

	glDisable(GL_TEXTURE_2D);
	glColor3ub(40, 40, 40);
	glBegin(GL_LINES);
	for (int x = 0; x <= worldWidth; x += cell)
	{
		glVertex2i(x, 0);
		glVertex2i(x, worldHeight);
	}
	for (int y = 0; y <= worldHeight; y += cell)
	{
		glVertex2i(0, y);
		glVertex2i(worldWidth, y);
	}
	glEnd();
	glColor3ub(255, 255, 255); // restore default so textured draws aren't tinted
	glEnable(GL_TEXTURE_2D);
}

// Draw the sprite rotated by the number of degrees specified by the angle parameter
void DrawUtilities::glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle)
{
	// Rotate the sprite about its own center by `angle` degrees on the modelview
	// matrix, then restore the previous transform.
	const float cx = x + w / 2.0f;
	const float cy = y + h / 2.0f;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(cx, cy, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);

	glDrawSprite(tex, x, y, w, h);

	glPopMatrix();
}
