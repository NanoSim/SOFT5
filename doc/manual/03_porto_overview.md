# NanoSim/Porto Overview
Porto is build on top of SOFT5 and is the "glue" between the different simulators that needs to publish and/or exchange data.

## Modularity and reusability

The Porto framework encourages a data driven approach to application
development. Instead of encapsulation and data hiding (as is the key
concept in object oriented programming), the data is public and
explicit. An effect of this is that individual models or offline
simulation codes that are developed can be very specialized (modular)
and need only to relate to the data and not (necessarily) to other
computational codes.

A key feature of Porto is to enable offline-coupling of physical
processes occurring on different scales.

## Key Features of the Porto platform.
The structure of the NanoSim platform is shown in Figure 1, where also the components of the two software platforms are visible. The Porto Core is a system that includes generic tools such as a code generator that allow for rapid development of interfaces, wrappers, and automatic programming of filtered models etc. 

The meta-data module is important, as all data that will be shared
between different processes and applications needs to be well
defined. The data storage module is the central database storage
system and the existing tool; MongoDB ( http://www.mongodb.org/), will
employed for this purpose. MongoDB is a data management and data
aggregation system and offers important features such as a free text
query system that is fast and scalable. MongoDB supports automatic
data partitioning for scalability and full index support.

A programming interface (API) for developing data format adaptors is
included in the user interface system of Porto to support 3rd party
data formats. When using the data interface module of Porto, the data
stored in separate formats are searchable and available through the
same interfaces as the data stored in the central data storage
system. . To further simplify the usage of these tools, the user
interface of Porto includes a scripting shell that can access all the
functionalities of the Porto framework. The scripting shell will also
be the central interface of the NanoSim platform. In addition, users
and developers will be able to interface with specific tools on the
NanoSim platform in ways they are familiar with (e.g., existing
graphical user interfaces for computational fluid dynamics software,
etc.). Future extensions of Porto might include a graphical user
interface in addition to the scripting shell.

## Data Driven Model Development using Porto
The Porto platform encourages a data driven approach to model (and
application) development. Instead of encapsulation and data hiding (as
is the key concept in object oriented programming), the data is public
and explicit. A unified interface for data retrieval and storage makes
data handling uniform across all applications. Because the data is
read-only (immutable) it is safe to access the same data across
several applications simultaneously, or by multiple researchers all
over the world at the same time. Since the models and applications
based on data managed by the Porto platform only need to relate to the
data interfaces, there are no limiting factors for the scalability.
Furthermore, meta-data based code generators can accelerate the code
development process by automatically building the code for generating
data, or running new models on the simulation software platform. The
implementation of logic and algorithms will be an isolated task that
allows for developing pure functions with well-defined
interfaces. This allows for good modular and reusable code with little
or no dependencies (loosely coupled). This approach brings truly
scalable, modular and reusable software that supports distributed data
exchange, interoperability and parallelization.  Functionality -
Connections between Scales using Offline Coupling via Porto While
there have been significant efforts to realize online coupling (e.g.,
coupling atomistic simulations to continuum-based models [14]), in
this project such an approach cannot be simply adopted for all
scales. This is due to the intertwined multi-phase physics and length
scales involved (e.g., discrete reactivity, reaction-diffusion
processes in a complex pore network, cluster/gas interaction with
simultaneous heat and mass transfer; Figure 2 illustrates the various
scales that will be studied) that makes online coupling at some scales
unattractive. Instead advanced offline coupling will be performed
using Porto for some scales. Specifically, the detailed understanding
gained from “resolved” simulations (e.g., DFT-based simulations using
the package VASP for atomistic processes, or discrete simulations
using CFDEMcoupling for multiphase flow simulations) will be used to
develop models describing the relevant phenomena at each scale. These
models will be implemented on the next coarser scale in order to
account for sub-scale effects in an efficient and robust way.  There
are two grand challenges connected to this offline coupling paradigm:
Firstly, in the model building phase, one needs to develop an
understanding of which processes need to be modelled and how, and
which data can be used to parameterize these models. Therefore an
ensemble of software tools for processing the detailed information at
8 the atomistic scale, sub-particle scale, or other small-scale
simulations will be developed as a central part of NanoSim. These
tools (called REMARC and C3PO, see next paragraphs) will be part of
the simulation software platform, but will be closely connected to
Porto. REMARC and C3PO will speed up and (partially) automate the
development of models for scale bridging via analysing and filtering
of simulation output. While such averaging has been performed in the
past, the key novelty now is the complete automation of this data
filtering process, making data analysis by researchers more efficient,
and bundling these features within a single platform that can interact
with various software packages. Clearly, with the help of REMARC and
C3PO, and their close connection to the Porto platform, researchers
can focus on their core tasks in model building, i.e., the improvement
of existing models and the development of new models by interpretation
of statistical data.  Secondly, the consistency between scales has to
be ensured, and in the context of offline coupling the following
reversibility checks will be performed: a newly developed model is
implemented in the next (more coarse) level, and corresponding
simulations using these models are run. This model implementation step
will be fully automated by the tools of Porto, such that there is no
overhead due to this step. Then, reference simulations on the original
(“fine scale”) level are performed using the same setup. Data from the
fine scale simulation is then filtered in order to generate statistics
similar to the data produced by the simulations on the coarse
level. It is now possible to directly compare the “fine scale” and
“coarse scale” simulation results, check if they match, and thereby
judge how effectively the model can bridge the two scales. The novelty
of the proposed procedure for reversibility checks is that Porto will
be able to set up these simulations automatically: the user only has
to pick which quantities need to be prepared, and Porto will generate
the appropriate input files (or scripts), as well as auto-generate
code (if necessary, e.g., specific modules for a new model) for the
simulations. This will allow researchers to focus on their primary
tasks, i.e., the interpretation of the differences between the
simulation results. Thus, it can be expected that the number and
quality of models produced by researchers will significantly increase.
Finally, it will be necessary to perform model validation based on
experimental data (from literature or from own experiments) because
certain model parameters are simply not available (e.g., it is not
possible to accurately obtain the pore network in a support particle,
or to obtain the spatial distribution of active nanoparticles within
the porous support). These validations will be performed at each stage
to demonstrate that the developed models are physically correct.
