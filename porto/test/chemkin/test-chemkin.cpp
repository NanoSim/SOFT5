#include <gtest/gtest.h>
#include <Soft>

#include "chemkinreaction.hxx"

TEST (ChemkinTest, Construct)
{
  auto storage = new soft::Storage("external", "chemkin:/home/sintef/source/soft5/porto/examples/remarc?chem=SurfaceChemkin.inp&thermo=thermo.dat", "");
  ASSERT_TRUE(nullptr != storage);
  auto collection = new soft::Collection();
  ASSERT_TRUE(nullptr != collection);

  soft::Chemkinreaction reaction_0(0,0,0,0,0);
  collection->attachEntity("reaction_0", &reaction_0);
  storage->load(collection);
  //storage->load(&reaction_0);
   
  std::cout << "Number of entities in collection: " << collection->numEntities()  << std::endl;
  std::cout << "Arrhenious #0" << " "
	    << reaction_0.A << " "    
	    << reaction_0.b << " "
	    << reaction_0.Ea << " "
	    << std::endl;
  
  delete collection;
  delete storage;
}
