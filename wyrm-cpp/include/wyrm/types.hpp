#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct WyrmDescription {
    int32_t     id;
    int32_t     parent_id;
    std::string name;
    int32_t     num_markers;

};

struct WyrmRigidBody {
    int32_t     id;
    std::string name;
    float   x, y, z;
    float   qx, qy, qz, qw;
    float   mean_error;
    bool    tracking_lost;
    bool    model_filled;
};

struct WyrmFrame {
    int32_t  frame_id;
    double   timestamp;
    uint32_t precision_timestamp_secs;
    uint32_t precision_timestamp_fractional_secs;
    bool     is_recording;
    bool     model_list_changed;
    uint32_t body_count;
    std::vector<WyrmRigidBody> bodies;
};
