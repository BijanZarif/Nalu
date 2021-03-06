/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#ifndef MomentumGclSrcNodeSuppAlg_h
#define MomentumGclSrcNodeSuppAlg_h

#include <SupplementalAlgorithm.h>
#include <FieldTypeDef.h>

#include <stk_mesh/base/Entity.hpp>

namespace sierra{
namespace nalu{

class Realm;

class MomentumGclSrcNodeSuppAlg : public SupplementalAlgorithm
{
public:

  MomentumGclSrcNodeSuppAlg(
    Realm &realm);

  virtual ~MomentumGclSrcNodeSuppAlg() {}

  virtual void setup();

  virtual void elem_execute(
    const int &numScvIntPoints,
    const int &numScsIntPoints,
    double *lhs,
    double *rhs,
    stk::mesh::Entity elem) {}

  virtual void node_execute(
    double *lhs,
    double *rhs,
    stk::mesh::Entity node);

  VectorFieldType *velocityNp1_;
  ScalarFieldType *densityNp1_;
  ScalarFieldType *divV_;
  ScalarFieldType *dualNodalVolume_;
  int nDim_;

};

} // namespace nalu
} // namespace Sierra

#endif
