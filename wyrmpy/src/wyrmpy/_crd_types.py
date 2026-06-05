"""
Copyright [2026] [Nicholas Sutton]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0
"""

from pycdr2 import IdlStruct
from pycdr2.types import int32, uint32, float32, float64, sequence
from dataclasses import dataclass

@dataclass
class _CdrRigidBody(IdlStruct):
    id:         int32
    name:       str
    x:          float32
    y:          float32
    z:          float32
    qx:         float32
    qy:         float32
    qz:         float32
    qw:         float32
    mean_error: float32
    flags:      uint32

@dataclass
class _CdrFrame(IdlStruct):
    frame_id:   int32
    timestamp:  float64
    pts_secs:   uint32
    pts_frac:   uint32
    flags:      uint32
    bodies:     sequence[_CdrRigidBody]

@dataclass
class _CdrDescription(IdlStruct):
    id:          int32
    parent_id:   int32
    name:        str
    num_markers: int32

@dataclass
class _CdrDescriptions(IdlStruct):
    descriptions: sequence[_CdrDescription]

