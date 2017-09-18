# SOFT5

SOFT5 is an acronym for SINTEF Open Framework and Tools no.5. SOFT5 is
a set of libraries and tools to support scientific software
development. 

The main approach to developing software with SOFT5 is to
incrementally describe the domain of the software through the use of
entities. An entity is a data model containing properties and
parameters. Since the entities must be formally specified, they can be
interpreted by computers and used for numerous tasks such as
code-generation, control I/O operations, documentation, version
handling etc.

The main idea of SOFT5 is that software may be written is such way
that business logic is handled by the codebase, while I/O,
file-formats, version handling, data import/export and
interoperability can be handled by reusable components in the
SOFT5-framework, thus reducing risk and development time. 



## Entities
The entity is an essential element in SOFT5. The entity shoud be self
contained (have high internal cohesion), and be independent of other
entities, in order to fulfill good software design principles. In web
ontology terms, the entity is meta data for describing
classes/datatypes, data properties, annotations, or combinations of
these. 

### Uniqueness
Each published entity needs to be uniquely identified in order to
avoid confusion. The entity identifier has therefore 3 separate
elements: a name, a namespace and a version number. An entity named
'Particle' is unlikely to have the same meaning and the set of
parameters across all domains. In particle physics, the entity
'Particle' would constitute matter and radiation, while in other
fields the term 'Particle' can be a general term to describe something
small. For this reason the SOFT5 entities have namespaces, similar to
how vocabularies are defined in OWL. The version number is a pragmatic
solution to handle how properties of an Entity might evolve during the
developmen process. In order to handle different versions of a
software, the entity version number can be used to identify the
necessary transformation between two data sets.

### Instances
When entities are instanciated, they are assigned a universally unique
identifier (UUID). This identifier is a 128-bit number that is used to
identify a specific piece of data/information.


## Collections
A collection is defined as a set of entities and relationships between
them. Collections are themself defined as entities, and can this
contain other collections as well. This is useful to represent the
knowledge of the domain where data exists, in order to find data that
relates to other data, but also to uniquely identify a complete data
set with a single identifier.

