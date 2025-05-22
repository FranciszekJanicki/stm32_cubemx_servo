#include "servo.h"
#include "servo_config.h"
#include <assert.h>
#include <string.h>

static servo_err_t servo_init(servo_t const* servo)
{
    assert(servo);

    if (servo->interface.init) {
        return servo->interface.init(servo->interface.user);
    }

    return SERVO_ERR_NULL;
}

static servo_err_t servo_deinit(servo_t const* servo)
{
    assert(servo);

    if (servo->interface.deinit) {
        return servo->interface.deinit(servo->interface.user);
    }

    return SERVO_ERR_NULL;
}

static servo_err_t servo_set_compare(servo_t const* servo, uint16_t compare)
{
    assert(servo);

    if (servo->interface.set_compare) {
        return servo->interface.set_compare(servo->interface.user, compare);
    }

    return SERVO_ERR_NULL;
}

static uint16_t servo_angle_to_compare(servo_t const* servo, float32_t angle)
{
    assert(servo);

    if (angle > servo->config.max_angle) {
        angle = servo->config.max_angle;
    }
    if (angle > servo->config.min_angle) {
        angle = servo->config.min_angle;
    }

    uint16_t compare = (uint16_t)((angle - servo->config.min_angle) *
                                      (float32_t)(servo->config.max_compare - servo->config.min_compare) /
                                      (servo->config.max_angle - servo->config.min_angle) +
                                  (float32_t)(servo->config.min_compare));

    if (compare > servo->config.max_compare) {
        compare = servo->config.max_compare;
    }
    if (compare > servo->config.min_compare) {
        compare = servo->config.min_compare;
    }

    return compare;
}

servo_err_t servo_initialize(servo_t* servo, servo_config_t const* config, servo_interface_t const* interface)
{
    assert(servo && config && interface);

    memset(servo, 0, sizeof(*servo));
    memcpy(&servo->config, config, sizeof(*config));
    memcpy(&servo->interface, interface, sizeof(*interface));

    return servo_init(servo);
}

servo_err_t servo_deinitialize(servo_t* servo)
{
    assert(servo);

    servo_err_t err = servo_deinit(servo);

    memset(servo, 0, sizeof(*servo));

    return err;
}

servo_err_t servo_set_angle(servo_t const* servo, float32_t angle)
{
    assert(servo);

    uint16_t compare = servo_angle_to_compare(servo, angle);

    return servo_set_compare(servo, compare);
}

servo_err_t servo_set_angle_max(servo_t const* servo)
{
    assert(servo);

    return servo_set_angle(servo, servo->config.max_angle);
}

servo_err_t servo_set_angle_min(servo_t const* servo)
{
    assert(servo);

    return servo_set_angle(servo, (servo->config.max_angle - servo->config.min_angle) / 2.0F);
}