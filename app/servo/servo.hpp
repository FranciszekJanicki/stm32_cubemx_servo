#ifndef SERVO_HPP
#define SERVO_HPP

#include "pwm_device.hpp"
#include "utility.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <utility>

namespace Servo {

    struct Servo {
        using PWMDevice = Utility::PWMDevice;

        void set_angle(float const angle) const noexcept;
        void set_angle_max() const noexcept;
        void set_angle_min() const noexcept;

        template <typename... floats>
        void run_sequence(floats const... angles) noexcept
        {
            for (auto const angle : std::array<float, sizeof...(angles)>{angles...}) {
                this->set_angle(angle);
                HAL_Delay(50);
            }
        }

        float (*angle_to_voltage)(float const);

        PWMDevice pwm_device{};
    };

}; // namespace Servo

#endif // SERVO_HPP