# SOFT5

SOFT5 is an acronym for SINTEF Open Framework and Tools no.5. SOFT5 is
a set of libraries and tools to support scientific software
development. 

The development of SOFT5 was motivated by many years of experience
with developing scientific software, where a lot of efforts went into
developing parts that had little to do with the domain. A significant
part of the development process is spent on software engineering
tasks, such as code design, the handling of I/O, correct memory
handling of the program state and writing import and export filters in
order to use data from different sources. In addition comes the code
maintenance with support of legacy formats and the introduction of new
features and data states. With SOFT5 it is possible to use reusable
software components that handle all this, or develop new reusable
software components that can be used by others in the same framework.

The main approach to developing software with SOFT5 is to
incrementally describe the domain of the software using entites (see
below). The entities can represent different elements of the software,
and be used in handling I/O as well as in code generation and
documentation. Entities can also be used for annotating data and data
sets. This might be useful in cases where for instance the origin of
the data, license and ownership are of importance.

Since any complex software will have many entities and often multiple
instances of the same entity, SOFT5 allows for creating collections of
entities with defined relationships. These entity collections are
called 'collections' (see below).

One idea of SOFT5 is that software may be written is such way that
business logic is handled by the codebase, while I/O, file-formats,
version handling, data import/export and interoperability can be
handled by reusable components in the SOFT5-framework, thus reducing
risk and development time.


## Entities
An entity contains information about data. It does not contain the
actual data, but describes what the different data fields are, in
terms of name, datatypes, units, dimensionality etc. Information about
data is often called meta data. Formal meta data enables for the
correct interpretation of a set of data, which otherwise would be
unreadable if not serialized in the correct and often undocumented
way.

An example of an entity is a 'Atom', which can be defined as something
that has a position, an atomic number (which characterizes the
chemical element), mass, charge, etc. Another entity can simply be a
reference-entity with properties such as name, description, license,
access-url, media-type, format, etc). The first entity is suitable as
an object in a simulation code, while the latter is more suitable for
a data catalog distribution description (see dcat:Distribution).
SOFT5 entities allows for describing many aspects of the domain. While
each entity describes a single unit of information, a collection of
entites can describe the complete domain. See collections below.

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

