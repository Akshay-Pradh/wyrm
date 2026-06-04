from ._crd_types import _CdrFrame, _CdrDescriptions
from .types import WyrmRigidBody, WyrmFrame, WyrmDescription

def deserialize_frame(data: bytes) -> WyrmFrame:
    raw = _CdrFrame.deserialize(data)
    return WyrmFrame(
        frame_id           = int(raw.frame_id),
        timestamp          = float(raw.timestamp),
        pts_secs           = int(raw.pts_secs),
        pts_frac           = int(raw.pts_frac),
        is_recording       = bool(raw.flags & 0x01),
        model_list_changed = bool(raw.flags & 0x02),
        body_count         = len(raw.bodies),
        bodies             = [
            WyrmRigidBody(
                id            = int(b.id),
                name          = b.name,
                x             = float(b.x),
                y             = float(b.y),
                z             = float(b.z),
                qx            = float(b.qx),
                qy            = float(b.qy),
                qz            = float(b.qz),
                qw            = float(b.qw),
                mean_error    = float(b.mean_error),
                tracking_lost = bool(b.flags & 0x01),
                model_filled  = bool(b.flags & 0x02),
            )
            for b in raw.bodies
        ]
    )

def deserialize_descriptions(data: bytes) -> dict[int, WyrmDescription]:
    raw = _CdrDescriptions.deserialize(data)
    return {
        int(d.id): WyrmDescription(
            id          = int(d.id),
            parent_id   = int(d.parent_id),
            name        = d.name,
            num_markers = int(d.num_markers),
        )
        for d in raw.descriptions
    }
