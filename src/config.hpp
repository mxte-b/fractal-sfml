#pragma once

namespace config {
    inline sf::Vector2u windowSize = sf::Vector2u{1920u, 1080u};
    inline sf::Vector2f windowSizeF = static_cast<sf::Vector2f>(windowSize);
    inline sf::Vector2i windowCenter = sf::Vector2i(windowSize.x / 2, windowSize.y / 2);
    inline constexpr uint32_t maxFrameRate = 144;
    inline constexpr bool isFullscreen = false;
}
