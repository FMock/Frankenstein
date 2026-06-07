#pragma once

#include "FrankensteinAPI.h"
#include <GL/glew.h>
#include <string>
#include <unordered_map>

/* TextureManager.h
 * Owns the OpenGL textures loaded by the engine. Loading goes through `load()`,
 * which caches by filename so the same image is uploaded to the GPU only once
 * (de-duplication), and `clear()` deletes every cached texture. This is what
 * fixes the previous leak: textures created via DrawUtilities::glTexImageTGAFile
 * were never freed. `clear()` must be called while a GL context is still current
 * (see Game::Shutdown).
 *
 * Implemented as a process-wide singleton because entities create their textures
 * inside their own constructors, without a handle to any owning object.
 */
class FRANKENSTEIN_API TextureManager
{
public:
	static TextureManager& instance();

	// Returns a cached texture handle for `filename`, loading + caching it on the
	// first request. Returns 0 if the image could not be loaded.
	GLuint load(const std::string& filename);

	// Deletes all cached GL textures and empties the cache. Requires a current GL context.
	void clear();

private:
	TextureManager() = default;
	std::unordered_map<std::string, GLuint> m_textures;
};
