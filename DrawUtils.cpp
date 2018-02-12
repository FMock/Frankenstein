/***********************************************************************
Utilities for loading and drawing sprites.
*/
#include<GL/glew.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

/* Load a file into an OpenGL texture, and return that texture. */
GLuint glTexImageTGAFile(const char* filename)
{
	const int BPP = 4;

	/* open the file */
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, "File: %s -- Could not open for reading.\n", filename);
		return 0;
	}

	/* skip first two bytes of data we don't need */
	fseek(file, 2, SEEK_CUR);

	/* read in the image type.  For our purposes the image type should
	* be either a 2 or a 3. */
	unsigned char imageTypeCode;
	fread(&imageTypeCode, 1, 1, file);
	if (imageTypeCode != 2 && imageTypeCode != 3) {
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

	/* allocate memory for image data and read it in */
	unsigned char* bytes = (unsigned char*)calloc(imageWidth * imageHeight * BPP, 1);

	/* read in data */
	if (bitCount == 32) {
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it) {
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = fgetc(file);
		}
	}
	else {
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it) {
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
void glDrawSprite(GLuint tex, int x, int y, int w, int h)
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
	glEnd();
}

/* Draw the sprite using only part of the texture.
 * The lower left of a texture is (0.0, 0.0) and upper right is (1.0, 1.0)
 * Horizontal axis is s, and vertical axis is t
 * param tex is an openGL texture mapped to a quad
 * param x is position to draw sprite in the game world
 * param y is position to draw sprite in the game world
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn
 * param s1 is left texture coordinate
 * param s2 is right texture coordinate
 * param t1 is lower texture coordinate
 * param t2 is upper texture coordinate
 * 
  A* * * * * *B
   *         *
   *         *
   *         *
  D* * * * * *C
 */
void glDrawSpriteSheet(GLuint tex, int x, int y, int w, int h, float s1, float s2, float t1, float t2){
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(s1, t2);  // A
		glVertex2i(x, y);

		glTexCoord2f(s2, t2);  // B
		glVertex2i(x + w, y);

		glTexCoord2f(s2, t1); // C
		glVertex2i(x + w, y + h);

		glTexCoord2f(s1, t1); // D
		glVertex2i(x, y + h);
	}
	glEnd();
}

// Draw the sprite rotated by the number of degrees specified by the angle parameter
void glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle){

	glMatrixMode(GL_TEXTURE); // make the texture matrix the target of matrix function calls

	glLoadIdentity();
	glPushMatrix();

	glRotatef(90.0, 0.0f, 0.0f, 1.0f);

	glDrawSprite(tex, x, y, w, h);

	glPopMatrix();

}
