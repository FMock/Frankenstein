#include "TextureManager.h"
#include "DrawUtils.h"
#include <vector>

TextureManager& TextureManager::instance()
{
	static TextureManager s_instance;
	return s_instance;
}

GLuint TextureManager::load(const std::string& filename)
{
	auto it = m_textures.find(filename);
	if (it != m_textures.end())
		return it->second;

	GLuint tex = DrawUtilities::glTexImageTGAFile(filename.c_str());
	// Cache even a 0 (failed) handle so we don't retry a missing file every frame.
	m_textures[filename] = tex;
	return tex;
}

void TextureManager::clear()
{
	std::vector<GLuint> handles;
	handles.reserve(m_textures.size());
	for (auto const& kv : m_textures)
	{
		if (kv.second != 0)
			handles.push_back(kv.second);
	}
	if (!handles.empty())
		glDeleteTextures(static_cast<GLsizei>(handles.size()), handles.data());
	m_textures.clear();
}
