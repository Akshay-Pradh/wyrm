"""
Copyright [2026] [Nicholas Sutton]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0
"""

from dataclasses import dataclass

@dataclass
class WyrmDescription():
    """ A Description Stores meta-data about a RigidBody """
    id:          int
    parent_id:   int
    name:        str
    num_markers: int

@dataclass
class WyrmRigidBody():
    """A RigidBody stores the tracking information for a specific body"""
    id:             int
    name:           str
    x:              float
    y:              float
    z:              float
    qx:             float
    qy:             float
    qz:             float
    qw:             float
    mean_error:     float
    tracking_lost:  bool
    model_filled:   bool

@dataclass
class WyrmFrame():
    """A Frame represents a frame of motion capture data. The frame holds metadata as well as, the Rigidbodies in the frame."""
    frame_id:               int
    timestamp:              float
    pts_secs:               int
    pts_frac:               int
    is_recording:           bool
    model_list_changed:     bool
    body_count:             int
    bodies:                 list[WyrmRigidBody]