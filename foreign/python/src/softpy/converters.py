# -*- coding: utf-8 -*-
from __future__ import absolute_import

from . import softpy
from .errors import SoftError
from .entity import BaseEntity

class SoftConverterError(SoftError):
    """Raised for conversion error."""
    pass

class SoftMissingConverterError(SoftConverterError):
    """Raised when a converter cannot be found."""
    pass


#
# Converter reference implementation
# ==================================
# FIXME: should be implemented in C++?
_converters = []

def register_converter(converter, inputs, outputs):
    """Registers a converter.

    Parameters
    ----------
    converter : callable
        A converter. It should take a sequence of input instances as
        argument and should return a sequence of output instances.
    inputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the converter takes as input.
    outputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the converter returns as output.
    """
    _converters.append((converter, inputs, outputs))


def _conversion_tree(output, inputs):
    """Returns a nested list structure describing how the entities in
    `inputs` can be converted to `output`.

    `output` is a (name, version, namespace)-tuple and `inputs` is a
    sequence of (name, version, namespace)-tuples.

    The returned value is either an element in `inputs` (if it matches
    `output`) or a nested tuple of the form

        (output, converter, [input, ...])

    where `input` is either an element in `inputs` or a tuple like above.

    Raises SoftMissingConverterError if none of the installed
    converters can convert `inputs` to `output`.
    """
    if output in inputs:
        return output
    for conv, ins, outs in _converters:
        if output in outs:
            try:
                return (output, conv, [_conversion_tree(inp, inputs)
                                       for inp in ins])
            except SoftMissingConverterError:
                pass
    raise SoftMissingConverterError('Cannot convert to %s' % (output, ))


def convert(output, input_instances):
    """Returns a new instance of (name, version, namespace) from the
    sequence of entity instances `input_instances`.

    Raises SoftMissingConverterError if none of the installed
    converters can convert `input_instances` to an instance of the
    desired type.
    """
    if isinstance(input_instances, BaseEntity):
        instances = [input_instances]
    inputdict = get_metadict(input_instances)
    if len(inputdict) != len(input_instances):
        raise SoftConverterError(
            'Converting from several instances of the same entity is ambiguous')

    def _convert(tree):
        if isinstance(tree[2], list):
            output, conv, inputs = tree
            assert output not in inputdict
            insts = conv([_convert(t) for t in inputs])
            d = get_metadict(insts)
            inputdict[output] = d[output]
            return d[output]
        else:
            return inputdict[tree]

    tree = _conversion_tree(output, inputdict.keys())
    return _convert(tree)


def get_metadict(instances):
    """Returns a dict mapping (name, version, namespace) of the metadata
    of each element in `instances` to the instances themselves.
    `instances` may also be a single instance."""
    if isinstance(instances, BaseEntity):
        instances = [instances]
    return {softpy.get_meta_mtype(inst): inst for inst in instances}
