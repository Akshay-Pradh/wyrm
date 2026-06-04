#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <wyrm/serialization.hpp>

uint32_t PackBodyFlags(const WyrmRigidBody& b) {
    uint8_t flags = 0;
    if (b.tracking_lost) flags |= 0x01;
    if (b.model_filled)  flags |= 0x02;
    return flags;
}

uint32_t PackFrameFlags(const WyrmFrame& f) {
    uint8_t flags = 0;
    if (f.is_recording)       flags |= 0x01;
    if (f.model_list_changed) flags |= 0x02;
    return flags;
}

void UnpackBodyFlags(uint8_t flags, WyrmRigidBody& b) {
    b.tracking_lost = flags & 0x01;
    b.model_filled  = flags & 0x02;
}

void UnpackFrameFlags(uint8_t flags, WyrmFrame& f) {
    f.is_recording       = flags & 0x01;
    f.model_list_changed = flags & 0x02;
}

std::vector<uint8_t> SerializeFrame(const WyrmFrame& frame) {
    eprosima::fastcdr::FastBuffer buffer;
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    cdr << frame.frame_id;
    cdr << frame.timestamp;
    cdr << frame.precision_timestamp_secs;
    cdr << frame.precision_timestamp_fractional_secs;
    cdr << static_cast<uint32_t>(PackFrameFlags(frame)); // This is cast to a u32 for alignment
    cdr << frame.body_count;

    for (const auto& body : frame.bodies) {
        cdr << body.id;
        cdr.serialize_array(body.name, MAX_NAME_LENGTH);
        cdr << body.x << body.y << body.z;
        cdr << body.qx << body.qy << body.qz << body.qw;
        cdr << body.mean_error;
        cdr << static_cast<uint32_t>(PackBodyFlags(body)); // This is cast to a u32 for alignment
    }

    return std::vector<uint8_t>(
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()),
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()) + cdr.get_serialized_data_length()
    );
}

std::vector<uint8_t> SerializeDescriptions(const std::unordered_map<int32_t, WyrmDescription>& descriptions) {
    eprosima::fastcdr::FastBuffer buffer;
    eprosima::fastcdr::Cdr cdr(buffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);

    cdr << static_cast<uint32_t>(descriptions.size());
    for (const auto& [id, desc] : descriptions) {
        cdr << desc.id;
        cdr << desc.parent_id;
        cdr.serialize_array(desc.name, MAX_NAME_LENGTH);
        cdr << desc.num_markers;
    }

    return std::vector<uint8_t>(
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()),
        reinterpret_cast<const uint8_t*>(buffer.getBuffer()) + cdr.get_serialized_data_length()
    );
}