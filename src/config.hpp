#pragma once

namespace config {
    inline sf::Vector2u windowSize = sf::Vector2u{1920u, 1080u};
    inline sf::Vector2f windowSizeF = static_cast<sf::Vector2f>(windowSize);
    inline uint32_t const maxFrameRate = 144;
    inline bool const isFullscreen = false;
}
