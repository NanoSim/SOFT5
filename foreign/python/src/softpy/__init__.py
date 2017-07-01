from __future__ import absolute_import

# FIXME - avoid namespace polution...
from .softpy import *

from .storage import Storage
from .collection import Collection
from .entity import entity, load_entity
from .metadata import (Metadata, register_metadb, register_jsondir,
                       find_metadata, find_metadata_uuid)
from .converters import register_converter, convert, get_metadict

__doc__ = softpy.__doc__
