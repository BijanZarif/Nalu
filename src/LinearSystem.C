/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#include <LinearSystem.h>
#include <EpetraLinearSystem.h>
#include <TpetraLinearSystem.h>
#include <ContactInfo.h>
#include <ContactManager.h>
#include <HaloInfo.h>
#include <Realm.h>
#include <Simulation.h>
#include <LinearSolver.h>
#include <master_element/MasterElement.h>

#include <stk_util/parallel/Parallel.hpp>
#include <stk_util/environment/CPUTime.hpp>

#include <stk_util/parallel/ParallelReduce.hpp>
#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Bucket.hpp>
#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/base/Selector.hpp>
#include <stk_mesh/base/GetBuckets.hpp>
#include <stk_mesh/base/Part.hpp>
#include <stk_topology/topology.hpp>
#include <stk_mesh/base/FieldParallel.hpp>

#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_FancyOStream.hpp>

#include <sstream>

namespace sierra{
namespace nalu{

//==========================================================================
// Class Definition
//==========================================================================
// LinearSystem - base class linear system
//==========================================================================
//--------------------------------------------------------------------------
//-------- constructor -----------------------------------------------------
//--------------------------------------------------------------------------
LinearSystem::LinearSystem(
  Realm &realm,
  const unsigned numDof,
  const std::string & name,
  LinearSolver *linearSolver)
  : realm_(realm),
    inConstruction_(false),
    writeCounter_(0),
    numDof_(numDof),
    name_(name),
    linearSolver_(linearSolver),
    linearSolveIterations_(0),
    nonLinearResidual_(0.0),
    linearResidual_(0.0),
    firstNonLinearResidual_(1.0e8),
    scaledNonLinearResidual_(1.0e8),
    recomputePreconditioner_(true),
    reusePreconditioner_(false),
    provideOutput_(true)
{
}

bool LinearSystem::debug()
{
  if (linearSolver_ && linearSolver_->root() && linearSolver_->root()->debug()) return true;
  return false;
}

// static method
LinearSystem *LinearSystem::create(Realm& realm, const unsigned numDof, const std::string & name, LinearSolver *solver)
{
  switch(solver->getType())
    {
    case PT_EPETRA:
      return new EpetraLinearSystem(realm,
                                    numDof,
                                    name,
                                    solver);
      break;
    case PT_TPETRA:
      return new TpetraLinearSystem(realm,
                                    numDof,
                                    name,
                                    solver);
      break;
    case PT_END:
    default:
      throw std::logic_error("create lin sys");
    }
  return 0;
}

void LinearSystem::sync_field(const stk::mesh::FieldBase *field)
{
  std::vector< const stk::mesh::FieldBase *> fields(1,field);
  stk::mesh::BulkData& bulkData = realm_.bulk_data();

  // only the aura = !locally_owned_part && !globally_shared_part (outer layer)
  //stk::mesh::communicate_field_data(bulkData.shared_aura(), fields);
  // the shared part (just the shared boundary)
  //stk::mesh::communicate_field_data(*bulkData.ghostings()[0], fields);
  stk::mesh::copy_owned_to_shared( bulkData, fields);
}

} // namespace nalu
} // namespace Sierra
