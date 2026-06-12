#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"

namespace dae
{
    class Texture2D;
    class Font;

    /// @brief Singleton that loads and caches engine assets (textures and fonts).
    ///
    /// All asset paths are resolved relative to the data directory supplied to Init().
    /// Both LoadTexture() and LoadFont() return shared_ptr handles; the manager
    /// retains a weak reference internally so that assets shared by multiple owners
    /// stay alive as long as at least one owner holds a handle.
    ///
    /// UnloadUnusedResources() (called internally) evicts cache entries whose
    /// shared_ptr has expired, keeping memory usage proportional to live assets.
    class ResourceManager final : public Singleton<ResourceManager>
    {
    public:
        /// @brief Sets the root directory from which all relative asset paths are resolved.
        ///        Must be called once before any Load* method.
        /// @param data Absolute or engine-relative path to the asset data directory.
        void Init(const std::filesystem::path& data);

        /// @brief Loads a texture from disk, or returns the cached instance if already loaded.
        ///
        /// The cache key is the @p file string as supplied. Callers should use consistent
        /// relative paths to benefit from caching.
        ///
        /// @param file Path to the image file, relative to the data directory.
        /// @return Shared ownership handle to the loaded Texture2D.
        std::shared_ptr<Texture2D> LoadTexture(const std::string& file);

        /// @brief Loads a font at a specific point size, or returns the cached instance.
        ///
        /// Font cache entries are keyed on both the file path and the point size, so
        /// the same font file at different sizes produces separate cache entries.
        ///
        /// @param file Path to the font file, relative to the data directory.
        /// @param size Desired font size in points.
        /// @return Shared ownership handle to the loaded Font.
        std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);

    private:
        friend class Singleton<ResourceManager>;
        ResourceManager() = default;

        /// @brief Removes cache entries for assets that are no longer referenced externally.
        void UnloadUnusedResources();

        std::filesystem::path m_dataPath; ///< Root directory prepended to all relative asset paths.

        std::map<std::string, std::shared_ptr<Texture2D>>               m_loadedTextures; ///< Texture cache keyed by file path.
        std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;   ///< Font cache keyed by (file path, point size).
    };

} // namespace dae