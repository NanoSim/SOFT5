# SOFT5

## Why SOFT5?

SOFT5 (SINTEF Open Framework and Tools) provides a lightweight and flexible system for data and workflow management.

This can be used to solve different difficult problems:

#### Coupling of multi-scale models (NanoSim)

The prevalent way of working with multi-scale model development is not data-agnostic. This complicates the adoption of new software and various workflows, as data has to be fitted to every transitional phase.

Existing systems that do embrace unified access to the data are often relying on predefined schemas. SOFT will overcomes these limitations by adopting technology from exemplary projects, tailored to fit the needs of the NanoSim project.


The key benefit of the unified data access is the ability for offline-analysis of big amounts of data by different research groups all over the world. Data
from one simulation can be used by various researchers to generate new model formulations, effectively removing the need for redundant simulations of the same problem. In addition, the seamless integration of statistical analysis tools will significantly speed up the model development process, which is currently a tedious and cumbersome task.



#### Coupling between simulator and user interface

The models OSCAR and DREAM are built on the MEMW framework. This provides a simulator (Fates) that performs the scientific calculations and a user interface (MEMW) for setting up scenarios and viewing results.

Transfering data between Fates and MEMW have traditionally ...



## Building and installation

Presently only the 64bit Linux platform is directly supported in SOFT5.

This guide assumes that the following dependencies has been configured and
installed:

* Qt 5.6  (http://qt-project.org/downloads)
* MongoDB 3.2.7 (http://www.mongodb.org/downloads)
* MongoDB C-Driver (http://docs.mongodb.org/ecosystem/drivers/c/)
* HDF5 1.8.17 (https://www.hdfgroup.org/HDF5/release/obtain5.html)

Additional optional dependencies needed for development:

* Google Test 1.7 (https://github.com/google/googletest/)
* Valgrind (http://valgrind.org/)
* Doxygen

The default SOFT build is configured with multiple options turned off. These options may have additional dependencies. See *Configuration options* below.

Be sure you have included the bin and lib directories of the above portoware tools in the PATH and LD_LIBRARY_PATH of your operating system.

Define the following environmental variables in your .bashrc or .cshrc

    SOFTBASE           = <path>/Porto/portobase
    LD_LIBRARY_PATH    = $PORTOBASE/lib:$PORTOBASE/bin/plugins:$LD_LIBRARY_PATH
    PATH               = $PORTOBASE/bin:$PATH
    QMAKEFEATURES      = $PORTOBASE/prf:$QMAKEFEATURES
    GTEST_LIBRARY_PATH = <path/to/gtest-lib>
    BSON_LIBRARY_PATH  = <path/to/libbson>
    MONGO_LIBRARY_PATH = <path/to/libmongo>


    export QTDIR=/opt/Qt/5.6/gcc_64
    export PATH=/usr/local/bin:$QTDIR/bin:/opt/Qt/Tools/QtCreator/bin/:$PATH
    export SOFTBASE=/home/sintef.no/petterr/usr/local/lib64/soft5
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SOFTBASE/plugins:$SOFTBASE/storage:$QTDIR/lib:/usr/local/lib:/usr/local/lib64
    export NO_AT_BRIDGE=1
    export PYTHONPATH=$PYTHONPATH:/usr/local/lib64/python2.7/site-packages/
    export BOOST_ROOT=/opt/boost/1.59.0/

Petters config:

    export QTDIR=/opt/Qt/5.6/gcc_64
    export HDF5DIR=/opt/hdf5
    export PATH="$GTESTDIR:$HDF5DIR:$QTDIR:$PATH"

For gtest installation, see for example (http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux) or use the system package installer.

For MongoDB installation, use for example the apt-get installation instructions on the MongoDB site.

For MongoDB C-Driver, see installation instructions here: (https://api.mongodb.com/c/current/installing.html)

For Doxygen generation, 


Build and install the application like this:

    cd <path>/Porto
    qmake
    make install
    
You should now have the following files:

    $PORTOBASE/bin/portoshell
    $PORTOBASE/bin/plugins/libmongoplugin.so
    $PORTOBASE/include/Porto
    $PORTOBASE/include/porto.h
    $PORTOBASE/include/portons.h
    $PORTOBASE/include/scriptengine.h

## Configuration options

TODO: Add CMake configuration options here.


## Where to go from here

To get started with Porto-development, please find the introductory [Porto Tutorial](doc/tutorial.org)
