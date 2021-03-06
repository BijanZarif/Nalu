/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#include <Realm.h>
#include <BoundaryConditions.h>
#include <NaluEnv.h>

// yaml for parsing..
#include <yaml-cpp/yaml.h>
#include <NaluParsing.h>

namespace sierra{
namespace nalu{

//==========================================================================
// Class Definition
//==========================================================================
// BoundaryCondition - do some stuff
//==========================================================================
//--------------------------------------------------------------------------
//-------- constructor -----------------------------------------------------
//--------------------------------------------------------------------------
  
//--------------------------------------------------------------------------
//-------- destructor ------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//-------- load -----------------------------------------------
//--------------------------------------------------------------------------


/// this is an example of a load() method with polymorphism - the type of
/// the node is determined from some information, then a particular type
/// of object is created and returned to the parent.

BoundaryCondition * BoundaryCondition::load(const YAML::Node & node) 
{
  if ( node.FindValue("wall_boundary_condition") ){
    WallBoundaryConditionData& wallBC = *new WallBoundaryConditionData(*parent());
    node >> wallBC;
    NaluEnv::self().naluOutputP0() << "Wall BC name:        " << wallBC.bcName_
                    << " on " << wallBC.targetName_ << std::endl;
    return &wallBC;
  }
  else if (node.FindValue("inflow_boundary_condition")) {
    InflowBoundaryConditionData& inflowBC = *new InflowBoundaryConditionData(*parent());
    node >> inflowBC;
    NaluEnv::self().naluOutputP0() << "Inflow BC name:      " << inflowBC.bcName_
                    << " on " << inflowBC.targetName_ << std::endl;
    return &inflowBC;
  }
  else if (node.FindValue("open_boundary_condition")) {
    OpenBoundaryConditionData& openBC = *new OpenBoundaryConditionData(*parent());
    node >> openBC;
    NaluEnv::self().naluOutputP0() << "Open BC name:        " << openBC.bcName_
                    << " on " << openBC.targetName_ << std::endl;
    return &openBC;
  }
  else if (node.FindValue("contact_boundary_condition")) {
    ContactBoundaryConditionData& contactBC = *new ContactBoundaryConditionData(*parent());
    node >> contactBC;
    NaluEnv::self().naluOutputP0() << "Contact BC name:     " << contactBC.bcName_
                    << " on " << contactBC.targetName_ << std::endl;
    return &contactBC;
  }
  else if (node.FindValue("symmetry_boundary_condition")) {
    SymmetryBoundaryConditionData& symmetryBC = *new SymmetryBoundaryConditionData(*parent());
    node >> symmetryBC;
    NaluEnv::self().naluOutputP0() << "Symmetry BC name:    " << symmetryBC.bcName_
                    << " on " << symmetryBC.targetName_ << std::endl;
    return &symmetryBC;
  }
  else if (node.FindValue("periodic_boundary_condition")) {
    PeriodicBoundaryConditionData& periodicBC = *new PeriodicBoundaryConditionData(*parent());
    node >> periodicBC;
    NaluEnv::self().naluOutputP0() << "Periodic BC name:    " << periodicBC.bcName_
                    << " between " << periodicBC.masterSlave_.master_
                    << " and "<< periodicBC.masterSlave_.slave_ << std::endl;
    return &periodicBC;
  }
  else if (node.FindValue("non_conformal_boundary_condition")) {
    NonConformalBoundaryConditionData& nonConformalBC = *new NonConformalBoundaryConditionData(*parent());
    node >> nonConformalBC;
    NaluEnv::self().naluOutputP0() << "NonConformal BC name:    " << nonConformalBC.bcName_
                    << " between " << nonConformalBC.masterSlave_.master_
                    << " and "<< nonConformalBC.masterSlave_.slave_ << std::endl;
    return &nonConformalBC;
  }
 
  else {
    throw std::runtime_error("parser error BoundaryConditions::load: no such bc type");
  }
  return 0;
}

  Simulation* BoundaryCondition::root() { return parent()->root(); }
  BoundaryConditions *BoundaryCondition::parent() { return &boundaryConditions_; }

  Simulation* BoundaryConditions::root() { return parent()->root(); }
  Realm *BoundaryConditions::parent() { return &realm_; }

} // namespace nalu
} // namespace Sierra
