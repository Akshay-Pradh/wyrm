"""
Copyright [2026] [Nicholas Sutton]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0
"""

from typing import Final

""" Key used to access the descriptions query through Zenoh """
WYRM_DESC_KEYEXPR: Final      = "wyrm/query/descriptions"

""" Key used to access the frame publisher through Zenoh """
WYRM_FRAME_KEYEXPR: Final     = "wyrm/pub/frame"