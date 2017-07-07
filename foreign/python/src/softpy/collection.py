# -*- coding: utf-8 -*-
from __future__ import absolute_import

from . import softpy
from .errors import SoftError
from .entity import entity
from .storage import Storage
from .metadata import find_metadata_uuid


class Collection(object):
    """A collection of entities with relations between them.

    With default arguments, it returns a new empty collection.

    If `uuid` is given, this will be the uuid of the new collection.
    This is normally used in combination with `driver`, `uri` and `options`
    to load the collection from a storage.

    See also: Storage
    """
    def __init__(self, uuid=None, driver=None, uri=None, options="append=yes"):
        if uuid:
            self.__soft_entity__ = softpy.collection_create(uuid)
        else:
            self.__soft_entity__ = softpy.collection_create_new()

        if driver:
            with Storage(driver=driver, uri=uri, options=options) as s:
                softpy.collection_load(s.storage, self.__soft_entity__)

        self._driver = driver
        self._uri = uri
        self._options = options

        # We store a Python-level references to all entities/collections
        # added to this collection.  This makes it possible to:
        #   - store the content of the collection together with the collection
        #   - retrieve elements added in this session without having their
        #     metadata in a in a metadata database
        self._cache = {}

    def __delete__(self):
        collection_free(self.__softpy_entity__)
        object.__del__(self)

    def add(self, label, entity):
        """Register a new entity associated with the given label."""
        e = softpy.get_c_entity(entity)
        if isinstance(e, softpy.entity_t):
            softpy.collection_register_entity(self.__soft_entity__, label, e)
        elif isinstance(e, softpy.collection_s):
            softpy.collection_register_collection(
                self.__soft_entity__, label, e)
        else:
            raise SoftError('Only instances, collections and metadata '
                            'can be added to collections.')
        self._cache[label] = entity

    #def pop(self, label):
    #    return collection_pop(self.__soft_entity__, label)

    #def remove(self, label):
    #    collection_remove(self.__soft_entity__, label)

    #def add_dim(self, label, description=''):
    #    collection_add_dim(self.__soft_entity__, label, description)

    def add_relation(self, subject, predicate, object_):
        softpy.collection_add_relation(self.__soft_entity__,
                                       subject, predicate, object_)

    #def connect(self, subject, predicate, object_):
    #    collection_connect(self.__soft_entity__,
    #                       subject, predicate, object_)

    def get_num_entities(self):
        return softpy.collection_num_entities(self.__soft_entity__)

    def get_num_relations(self):
        """Returns the total number of relations associated with this
        collection.

        Note: A collection will typically contain many more relations
        than the number of relations you have added with add_relation()
        because relations are also used to associate entity labels with
        the entity type, name, version , namespace and uuid."""
        return softpy.collection_num_relations(self.__soft_entity__)

    #def get_num_dim_maps(self):
    #     return collection_num_dim_maps(self.__soft_entity__)
    #
    #def get_dimensions(self):
    #     return collection_get_dimensions(self.__soft_entity__)

    def get_labels(self):
        """Returns a set with all registered entity labels."""
        return self.find_relations('Entity', '^is-a')

    def get_uuid(self, label):
        """Returns the uuid of the entity instance with the given label."""
        uuid, = self.find_relations(label, 'id')
        return uuid

    def get_name(self, label):
        """Returns the name of the entity instance with the given label."""
        name, = self.find_relations(label, 'name')
        return name

    def get_version(self, label):
        """Returns the version of the entity instance with the given label."""
        version, = self.find_relations(label, 'version')
        return version

    def get_namespace(self, label):
        """Returns the namespace of the entity instance with the given
        label."""
        namespace, = self.find_relations(label, 'namespace')
        return namespace

    def get_instance(self, label, driver=None, uri=None, options=None):
        """Returns the the entity instance associated with `label`
        from the storage specified with `driver`, `uri` and `options`.

        Default values for `driver`, `uri` and `options` can be set
        with the corresponding properties.  If this collection was
        loaded from a storage, these properties be initialised to the
        same storage.

        This method depends on that the metadata for `label` exists in
        a metadata database registered with softpy.register_metadb().
        """
        if label in self._cache:
            return self._cache[label]
        driver = driver if driver else self._driver
        uri = uri if uri else self._uri
        options = options if options else self._options
        if not driver or not uri:
            raise ValueError('`driver` and `uri` must be given when no '
                             'defaults are set')
        name = self.get_name(label)
        version = self.get_version(label)
        namespace = self.get_namespace(label)
        uuid = self.get_uuid(label)
        if (name, namespace) == ('Collection', 'org.sintef.soft'):
            return Collection(uuid=uuid, driver=driver, uri=uri,
                              options=options)
        elif (name, namespace) == ('entity_schema', 'org.sintef.soft'):
            meta = find_metadata_uuid(uuid)
            return entity(meta)
        else:
            cls = entity(name, version, namespace)
            return cls(uuid=uuid, driver=driver, uri=uri, options=options)

    name = property(
        lambda self: softpy.collection_get_name(self.__soft_entity__),
        lambda self, name: softpy.collection_set_name(
            self.__soft_entity__, name),
        doc='Collection name.'
    )

    version = property(
        lambda self: softpy.collection_get_version(self.__soft_entity__),
        lambda self, version: softpy.collection_set_version(
            self.__soft_entity__, version),
        doc='Collection version.'
    )

    namespace = property(
        lambda self: softpy.collection_get_namespace(self.__soft_entity__),
        lambda self, namespace: softpy.collection_set_namespace(
            self.__soft_entity__, namespace),
        doc='Collection name space.'
    )

    uuid = property(
        lambda self: self.__soft_entity__.id,
        doc='The uuid of this collection.')

    driver = property(
        lambda self: self._driver,
        lambda self, value: setattr(self, '_driver', value),
        doc='Default driver used by get_entities().')

    uri = property(
        lambda self: self._uri,
        lambda self, value: setattr(self, '_uri', value),
        doc='Default uri used by get_entities().')

    options = property(
        lambda self: self._options,
        lambda self, value: setattr(self, '_options', value),
        doc='Default options used by get_entities().')

    def find_relations(self, subject, predicate):
        """Returns a set with all relations matching the given `subject` and
        `predicate`.

        If `predicate` is preceded with "^", the match is inverted, i.e.
        all relations whos object matches `subject` and predicate matches
        the remaining of `predicate` are returned."""
        strlst = softpy.collection_find_relations(self.__soft_entity__,
                                                  subject, predicate)
        relations = set()
        for i in range(softpy.string_list_count(strlst)):
            relations.add(softpy.string_at(strlst, i))
        softpy.string_list_free(strlst)
        return relations

    def save(self, storage=None, store_content=True):
        """A convenient method for storing a collection.

        If `storage` is not given, the default storage will be used.

        If `store_content` is true, the content of the collection will
        also be stored.

        Note: if `storage` is given and `store_content` is true, the
        storage should have been created with option "append=yes".
        """
        if storage:
            pass  # ok, proceed...
        elif self.driver and self.uri:
            with Storage(self.driver, self.uri, self.options) as s:
                return self.save(s, store_content)
        else:
            raise ValueError('`storage` must be given when no default storage')

        if store_content:
            for label, e in self._cache.items():
                if isinstance(e, Collection):
                    e.save(storage, store_content=store_content)
                else:
                    storage.save(e)
        storage.save(self)
