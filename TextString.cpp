#include "TextString.h"

TextString::TextString(){} // mandatory default constructor required by VS

/* Constructs a TextString
* param image - the GLuint image int
* param w - width of the image
* param h - height of the image
* param fw - width of frame within image
* param fh - height of frame within image
*/
TextString::TextString(char str[], GLuint image, int w, int h, int fw, int fh, int x, int y) : string(str), 
																							   image(image), 
																							   width(w), height(h), 
																							   frameWidth(fw), 
																							   frameHeight(fh), 
																							   numColumns(w/fw), 
																							   numRows(h/fh), 
																							   x(x), 
																							   y(y){

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

	float colDivision = 1.0/numColumns;
	float rowDivision = 1.0/numRows;
	int currentCol = 0;
	int currentRow = 0;

	for(int i = 0; i < strlen(string); i++){

		int asciiValue = string[i]; // get ascii value of character

		currentCol = (asciiValue - 32) % 16;
		currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

		// update s1, s2, t1, t2
		s1 = currentCol * colDivision;
		s2 = (currentCol * colDivision) + colDivision;
		t1 = currentRow * rowDivision;
		t2 = (currentRow * rowDivision) + rowDivision;

		glDrawFrame(image, x + i * (frameWidth/2), y, frameWidth, frameHeight, s1, s2, t1, t2);
	}
}