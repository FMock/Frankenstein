#include"animation.h"

/* Animation
* param image - The openGL texture that represents the frames of the animation
* param a - numberOfFrames, the number of frames in one row of the animation. Multi row
*           animations must contain the same number of frames in each row
* param b - numberOfRows, the number of rows this animation consists of
* param c - startingRow, the row this animation starts at on the spritesheet
* param d - name of this animation */
Animation::Animation(GLuint image, int a, int b, int c, const std::string& d, const int facingDir): m_image(image),
					m_numberOfFrames(a), m_numberOfRows(b), m_frameDivision(1.0/m_numberOfFrames),
					m_rowDivision(1.0/m_numberOfRows), m_startingRow(c), m_currentFrame(0),
					m_currentRow(m_startingRow), m_s1(0.0), m_s2(m_frameDivision), m_t1(m_currentRow*m_rowDivision), 
					m_t2((m_currentRow*m_rowDivision) + m_rowDivision), m_name(d), m_facingDirection(facingDir){}

Animation::Animation(AnimationParameters& params, SpriteSheetParameters& spriteSheetParams, const std::string& name, const int facingDirection) :

	m_spriteSheetInfo(spriteSheetParams),
	m_image(params.image),
	m_numberOfFrames(params.framesInAnimation),
	m_numberOfRows(params.rowsInAnimation),
	m_startingRow(params.startingRow),
	m_startingCol(params.startingCol),
	m_frameDivision(1.0 / (m_spriteSheetInfo.m_spriteSheetWidth / m_spriteSheetInfo.m_frameWidth)),
	m_rowDivision(1.0 / (m_spriteSheetInfo.m_spriteSheetHeight / m_spriteSheetInfo.m_frameHeight)),
	m_currentFrame(m_startingCol),
	m_currentRow(m_startingRow),
	m_s1(0.0),
	m_s2(m_frameDivision),
	m_t1(m_currentRow* m_rowDivision),
	m_t2((m_currentRow* m_rowDivision) + m_rowDivision),
	m_name(name),
	m_facingDirection(facingDirection)
{
}

Animation::Animation(GLuint image, int a, int b, int c, int c2, SpriteSheetParameters& spriteSheetParams, const std::string& d, const int facingDirection) :
	m_image(image),
	m_numberOfFrames(a),
	m_numberOfRows(b),
	m_startingRow(c),
	m_startingCol(c2),
	m_spriteSheetInfo(spriteSheetParams),
	m_frameDivision(1.0 / (m_spriteSheetInfo.m_spriteSheetWidth / m_spriteSheetInfo.m_frameWidth)),
	m_rowDivision(1.0 / (m_spriteSheetInfo.m_spriteSheetHeight / m_spriteSheetInfo.m_frameHeight)),
	m_currentFrame(m_startingCol),
	m_currentRow(m_startingRow),
	m_s1(0.0),
	m_s2(m_frameDivision),
	m_t1(m_currentRow* m_rowDivision),
	m_t2((m_currentRow* m_rowDivision) + m_rowDivision),
	m_name(d), m_facingDirection(facingDirection)
{
}

void Animation::nextFrame(float frameTime, float animFPS){
	// Calculate how many frames to jump ahead
	m_currentFrame += frameTime * animFPS;

	// Keep currentFrame in range
	if (m_currentFrame >= m_numberOfFrames)
		m_currentFrame = m_currentFrame % m_numberOfFrames;

	// current row may change. Use modulus
	if (m_s2 >= 1.0 && m_numberOfRows > 1)
		m_currentRow = (m_currentRow + 1) % m_numberOfRows;

	// update s1, s2, t1, t2
	if (m_numberOfFrames == 1)
	{
		m_s1 = m_startingCol * m_frameDivision;
		m_s2 = (m_startingCol * m_frameDivision) + m_frameDivision;
	}
	else
	{
		m_s1 = m_currentFrame * m_frameDivision;
		m_s2 = (m_currentFrame * m_frameDivision) + m_frameDivision;
	}

	m_t1 = m_currentRow * m_rowDivision;
	m_t2 = (m_currentRow * m_rowDivision) + m_rowDivision;
}
