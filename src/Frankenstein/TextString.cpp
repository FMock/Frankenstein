#include "TextString.h"
#include "Game.h"

using namespace DrawUtilities;

TextString::TextString(){} // mandatory default constructor required by VS
TextString::TextString(Game* game) : m_game(game){}

void TextString::Initialize(const char* string, TextStringInitParams& params)
{
	m_string = string;
	m_image = params.image;
	m_width = params.imageWidth;
	m_height = params.imageHeight;
	m_frameWidth = params.frameWidth;
	m_frameHeight = params.frameHeight;
	m_numColumns = (params.imageWidth / params.frameWidth);
	m_numRows = (params.imageHeight / params.frameHeight);
	m_x = params.x;
	m_y = params.y;
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

		GlDrawFrameParams params;

		params.tex = m_image;
		params.x = m_x + i * (m_frameWidth / 2);
		params.y = m_y;
		params.w = m_frameWidth;
		params.h = m_frameHeight;
		params.s1 = m_s1;
		params.s2 = m_s2;
		params.t1 = m_t1;
		params.t2 = m_t2;

		glDrawFrame(params);
	}
}