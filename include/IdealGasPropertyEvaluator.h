/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#ifndef IdealGasPropertyEvaluator_h
#define IdealGasPropertyEvaluator_h

#include <PropertyEvaluator.h>
#include<FieldTypeDef.h>

#include <vector>

namespace stk { namespace mesh { struct Entity; } }

namespace stk {
namespace mesh {
class MetaData;
}
}

namespace sierra{
namespace nalu{

class IdealGasTPropertyEvaluator : public PropertyEvaluator
{
public:

  IdealGasTPropertyEvaluator(
    double pRef,
    double universalR,
    std::vector<std::pair<double, double> > mwMassFracVec);
  virtual ~IdealGasTPropertyEvaluator();
  
  double execute(
    double *indVarList,
    stk::mesh::Entity node);
  
  const double pRef_;
  const double R_;
  double mw_;
  
};

class IdealGasTYkPropertyEvaluator : public PropertyEvaluator
{
public:

  IdealGasTYkPropertyEvaluator(
      double pRef,
      double universalR,
      std::vector<double>mwVec,
      stk::mesh::MetaData &metaData);
  virtual ~IdealGasTYkPropertyEvaluator();
  
  double execute(
      double *indVarList,
      stk::mesh::Entity node);
  
  double compute_mw(
      const double *yk);
  
  // reference quantities
  const double pRef_;
  const double R_;  

  // field definition and extraction,
  GenericFieldType *massFraction_;

  // reference mw vector; size and declaration
  size_t mwVecSize_;
  std::vector<double> mwVec_;
  
};

class IdealGasTPPropertyEvaluator : public PropertyEvaluator
{
public:

  IdealGasTPPropertyEvaluator(
      double universalR,
      std::vector<std::pair<double, double> > mwMassFracVec,
      stk::mesh::MetaData &metaData);
  virtual ~IdealGasTPPropertyEvaluator();

  double execute(
      double *indVarList,
      stk::mesh::Entity node);

  // reference quantities
  const double R_;

  // molecular weight computed at construction time
  double mw_;

  // field definition and extraction,
  ScalarFieldType *pressure_;

};

class IdealGasYkPropertyEvaluator : public PropertyEvaluator
{
public:

  IdealGasYkPropertyEvaluator(
      double pRef,
      double tRef,
      double universalR,
      std::vector<double>mwVec,
      stk::mesh::MetaData &metaData);
  virtual ~IdealGasYkPropertyEvaluator();
  
  double execute(
      double *indVarList,
      stk::mesh::Entity node);
  
  double compute_mw(
      const double *yk);
  
  // reference quantities
  const double pRef_;
  const double tRef_;
  const double R_;  

  // field definition and extraction,
  GenericFieldType *massFraction_;

  // reference mw vector; size and declaration
  size_t mwVecSize_;
  std::vector<double> mwVec_;
  
};

} // namespace nalu
} // namespace Sierra

#endif
