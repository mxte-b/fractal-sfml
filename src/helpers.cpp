#include "helpers.hpp"

#include <iostream>

#include "config.hpp"
#include "SFML/Graphics/View.hpp"
void updateShader(sf::Shader &shader, const raymarch::Camera &camera, const float iTime)
{
    shader.setUniform("camPosition", camera.getPosition());
    shader.setUniform("camRotationMatrix", camera.getRotationMatrix());
    shader.setUniform("fov", camera.getFOV());
    shader.setUniform("aperture", camera.getAperture());
    shader.setUniform("focusDistance", camera.getFocusDistance());
    shader.setUniform("iTime", iTime);
}

std::string getDateTimeString()
{
    // Get current time as time_t
    const auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert to tm struct for local time
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now_time_t);  // Windows version (thread-safe)
#else
    localtime_r(&now_time_t, &local_tm);  // POSIX version (thread-safe)
#endif

    // Format time to string, e.g. "2025-07-22_15-30-45"
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d_%H-%M-%S");

    return oss.str();
}