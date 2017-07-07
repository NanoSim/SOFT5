# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

from . import softpy
from .errors import SoftError

class SoftTranslatorError(SoftError):
    """Raised for translation error."""
    pass

class SoftMissingTranslatorError(SoftTranslatorError):
    """Raised when a translator cannot be found."""
    pass


#
# Translator reference implementation
# ==================================
# FIXME: should be implemented in C++?
_translators = []

def register_translator(translator, inputs, outputs):
    """Registers a translator.

    Parameters
    ----------
    translator : callable
        A translator. It should take a sequence of input instances as
        argument and should return a sequence of output instances.
    inputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the translator takes as input.
    outputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the translator returns as output.
    """
    _translators.append((translator, inputs, outputs))


def _translation_tree(output, inputs):
    """Returns a nested list structure describing how the entities in
    `inputs` can be translated to `output`.

    `output` is a (name, version, namespace)-tuple and `inputs` is a
    sequence of (name, version, namespace)-tuples.

    The returned value is either an element in `inputs` (if it matches
    `output`) or a nested tuple of the form

        (output, translator, [input, ...])

    where `input` is either an element in `inputs` or a tuple like above.

    Raises SoftMissingTranslatorError if none of the installed
    translators can translate `inputs` to `output`.
    """
    if output in inputs:
        return output
    for trans, ins, outs in _translators:
        if output in outs:
            try:
                return (output, trans, [_translation_tree(inp, inputs)
                                       for inp in ins])
            except SoftMissingTranslatorError:
                pass
    raise SoftMissingTranslatorError('Cannot translate to %s' % (output, ))


def translate(output, input_instances):
    """Returns a new instance of type `output` from `input_instances`.

    Args:
        output: A (name, version, namespace)-tuple specifying the desired
            type.
        input_instances: Sequence of input entity instances.

    Returns:
        New instance of the desired type populated from `input_instances`.

    Raises:
        SoftMissingTranslatorError: If none of the installed
            translators can translate `input_instances` to an instance
            of the desired type.
    """
    if hasattr(input_instances, 'soft_get_id'):
        instances = [input_instances]
    inputdict = get_metadict(input_instances)
    if len(inputdict) != len(input_instances):
        raise SoftTranslatorError(
            'Translating from several instances of the same type is '
            'ambiguous')

    def _translate(tree):
        if isinstance(tree[2], list):
            output, trans, inputs = tree
            assert output not in inputdict
            insts = trans([_translate(t) for t in inputs])
            d = get_metadict(insts)
            inputdict[output] = d[output]
            return d[output]
        else:
            return inputdict[tree]

    tree = _translation_tree(output, inputdict.keys())
    return _translate(tree)


def get_metadict(instances):
    """Returns a dict mapping (name, version, namespace) of the metadata
    of each element in `instances` to the instances themselves.
    `instances` may also be a single instance."""
    if hasattr(instances, 'soft_get_id'):
        instances = [instances]
    return {softpy.get_meta_mtype(inst): inst for inst in instances}
