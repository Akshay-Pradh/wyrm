/**
 * Copyright [2026] [Nicholas Sutton]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#pragma once

#include <string>
#include <vector>
#include <cstdint>

/** A Description Stores meta-data about a RigidBody */
struct WyrmDescription {
    int32_t     id;
    int32_t     parent_id;
    std::string name;
    int32_t     num_markers;

};

/** A RigidBody stores the tracking information for a specific body */
struct WyrmRigidBody {
    int32_t     id;
    std::string name;
    float   x, y, z;
    float   qx, qy, qz, qw;
    float   mean_error;
    bool    tracking_lost;
    bool    model_filled;
};

/** 
 * A Frame represents a frame of motion capture data. The frame holds metadata as well as, the Rigidbodies in the frame.
 */
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
