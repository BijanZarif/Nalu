/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#ifndef HeatCondMassBDF2NodeSuppAlg_h
#define HeatCondMassBDF2NodeSuppAlg_h

#include <SupplementalAlgorithm.h>
#include <FieldTypeDef.h>

#include <stk_mesh/base/Entity.hpp>

namespace sierra{
namespace nalu{

class Realm;

class HeatCondMassBDF2NodeSuppAlg : public SupplementalAlgorithm
{
public:

  HeatCondMassBDF2NodeSuppAlg(
    Realm &realm);

  virtual ~HeatCondMassBDF2NodeSuppAlg() {}

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

  ScalarFieldType *temperatureNm1_;
  ScalarFieldType *temperatureN_;
  ScalarFieldType *temperatureNp1_;
  ScalarFieldType *density_;
  ScalarFieldType *specificHeat_;
  ScalarFieldType *dualNodalVolume_;
  double dt_;
  double gamma1_, gamma2_, gamma3_;

};

} // namespace nalu
} // namespace Sierra

#endif
