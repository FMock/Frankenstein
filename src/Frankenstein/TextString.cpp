#include "TextString.h"

TextString::TextString(){} // mandatory default constructor required by VS

void TextString::Initialize(const char* string, GLuint image, int imageWidth, int imageHeight, int frameWidth, int frameHeight, int x, int y)
{
	m_string = string;
	m_image = image;
	m_width = imageWidth;
	m_height = imageHeight;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_numColumns = (imageWidth / frameWidth);
	m_numRows = (imageHeight / frameHeight);
	m_x = x;
	m_y = y;
}

/*Draws each character of this objects string
 *Assumes the following raster font image which is 16 characters wide X 6 characters high
 character in the top left corner is a space, ascii character = 32

   !"#$%&'()*+,-./
  0123456789:;<=>?
  @ABCDEFGHIJKLMNO
  PQRSTUVWXYZ[\]^_
  `abcdefghijklmno
  qrstuvwxyz{|}~
 * */
void TextString::drawText(){

	float colDivision = 1.0f/m_numColumns;
	float rowDivision = 1.0f/m_numRows;
	int currentCol = 0;
	int currentRow = 0;

	for(int i = 0; i < strlen(m_string); i++){

		int asciiValue = m_string[i]; // get ascii value of character

		currentCol = (asciiValue - 32) % 16;
		currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

		// update s1, s2, t1, t2
		m_s1 = currentCol * colDivision;
		m_s2 = (currentCol * colDivision) + colDivision;
		m_t1 = currentRow * rowDivision;
		m_t2 = (currentRow * rowDivision) + rowDivision;

		glDrawFrame(m_image, m_x + i * (m_frameWidth/2), m_y, m_frameWidth, m_frameHeight, m_s1, m_s2, m_t1, m_t2);
	}
}