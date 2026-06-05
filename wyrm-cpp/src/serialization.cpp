/**
 * Copyright [2026] [Nicholas Sutton]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <wyrm/serialization.hpp>

/**
 * Converts boolean flags in a RigidBody to a uint32
 * @param b RigidBody whose flags we convert
 * @return a uint32 whos bytes match the flags
 */
uint32_t PackBodyFlags(const WyrmRigidBody& b) {
    uint32_t flags = 0;
    if (b.tracking_lost) flags |= 0x01;
    if (b.model_filled)  flags |= 0x02;
    return flags;
}

/**
 * Converts boolean flags in a Frame to a uint32
 * @param f frame whose flags we convert
 * @return a uint32 whos bytes match the flags
 */
uint32_t PackFrameFlags(const WyrmFrame& f) {
    uint32_t flags = 0;
    if (f.is_recording)       flags |= 0x01;
    if (f.model_list_changed) flags |= 0x02;
    return flags;
}

/**
 * Converts a uint32 of flags in a RigidBody to a boolean flags
 * @param flags A uint32 whose bytes are flags
 * @param b RigidBody whose flags we set
 */
void UnpackBodyFlags(uint32_t flags, WyrmRigidBody& b) {
    b.tracking_lost = flags & 0x01;
    b.model_filled  = flags & 0x02;
}

/**
 * Converts a uint32 of flags in a Frame to a boolean flags
 * @param flags A uint32 whose bytes are flags
 * @param f frame whose flags we set
 */
void UnpackFrameFlags(uint32_t flags, WyrmFrame& f) {
    f.is_recording       = flags & 0x01;
    f.model_list_changed = flags & 0x02;
}

/**
 * Serializes a WyrmFrame struct using FastCDR
 * @param frame The frame to serialize
 * @return A vector of bytes
 */
std::vector<uint8_t> SerializeFrame(const WyrmFrame& frame) {
    eprosima::fastcdr::FastBuffer buffer;
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    cdr << frame.frame_id;
    cdr << frame.timestamp;
    cdr << frame.precision_timestamp_secs;
    cdr << frame.precision_timestamp_fractional_secs;
    cdr << PackFrameFlags(frame);
    cdr << frame.body_count;

    for (const auto& body : frame.bodies) {
        cdr << body.id;
        cdr << body.name;
        cdr << body.x << body.y << body.z;
        cdr << body.qx << body.qy << body.qz << body.qw;
        cdr << body.mean_error;
        cdr << PackBodyFlags(body);
    }

    return std::vector<uint8_t>(
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()),
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()) + cdr.get_serialized_data_length()
    );
}

/**
 * Serializes a WyrmDescriptions Map using FastCDR
 * @param frame The descriptions to serialize
 * @return A vector of bytes
 */
std::vector<uint8_t> SerializeDescriptions(const std::unordered_map<int32_t, WyrmDescription>& descriptions) {
    eprosima::fastcdr::FastBuffer buffer;
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    cdr << static_cast<uint32_t>(descriptions.size());
    for (const auto& [id, desc] : descriptions) {
        cdr << desc.id;
        cdr << desc.parent_id;
        cdr << desc.name;
        cdr << desc.num_markers;
    }

    return std::vector<uint8_t>(
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()),
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()) + cdr.get_serialized_data_length()
    );
}

/**
 * Deserializes a WyrmFrame struct using FastCDR
 * @param data a vector of bytes to deserialize
 * @return A WyrmFrame Struct
 */
WyrmFrame DeserializeFrame(const std::vector<uint8_t>& data) {
    eprosima::fastcdr::FastBuffer buffer(
        reinterpret_cast<char*>(const_cast<uint8_t*>(data.data())),
        data.size()
    );
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    WyrmFrame frame;
    uint32_t flags;

    cdr >> frame.frame_id;
    cdr >> frame.timestamp;
    cdr >> frame.precision_timestamp_secs;
    cdr >> frame.precision_timestamp_fractional_secs;
    cdr >> flags;
    UnpackFrameFlags(flags, frame);
    cdr >> frame.body_count;

    frame.bodies.resize(frame.body_count);
    for (auto& body : frame.bodies) {
        uint32_t body_flags;
        cdr >> body.id;
        cdr >> body.name;
        cdr >> body.x >> body.y >> body.z;
        cdr >> body.qx >> body.qy >> body.qz >> body.qw;
        cdr >> body.mean_error;
        cdr >> body_flags;
        UnpackBodyFlags(body_flags, body);
    }

    return frame;
}

/**
 * Deserializes a WyrmDescriptions Map using FastCDR
 * @param data a vector of bytes to deserialize
 * @return A WyrmDescriptions Map
 */
std::unordered_map<int32_t, WyrmDescription> DeserializeDescriptions(const std::vector<uint8_t>& data) {
    eprosima::fastcdr::FastBuffer buffer(
        reinterpret_cast<char*>(const_cast<uint8_t*>(data.data())),
        data.size()
    );
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    std::unordered_map<int32_t, WyrmDescription> descriptions;
    uint32_t count;
    cdr >> count;

    for (uint32_t i = 0; i < count; i++) {
        WyrmDescription desc;
        cdr >> desc.id;
        cdr >> desc.parent_id;
        cdr >> desc.name;
        cdr >> desc.num_markers;
        descriptions[desc.id] = desc;
    }

    return descriptions;
}