/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#ifndef Realm_h
#define Realm_h

#include <Enums.h>
#include <FieldTypeDef.h>

// yaml for parsing..
#include <yaml-cpp/yaml.h>

#include <BoundaryConditions.h>
#include <InitialConditions.h>
#include <MaterialPropertys.h>
#include <EquationSystems.h>
#include <Teuchos_RCP.hpp>

#include <stk_util/util/ParameterList.hpp>

// standard c++
#include <map>
#include <string>
#include <vector>
#include <stdint.h>

namespace stk {
namespace mesh {
class Part;
}
namespace io {
  class StkMeshIoBroker;
}
}

namespace YAML {
class Node;
}

namespace sierra{
namespace nalu{

class Algorithm;
class AlgorithmDriver;
class AuxFunctionAlgorithm;
class ComputeGeometryAlgorithmDriver;
class ContactInfo;
class ContactManager;
class NonConformalManager;
class ErrorIndicatorAlgorithmDriver;
#if defined (NALU_USES_PERCEPT)
class Adapter;
#endif
class EquationSystems;
class OutputInfo;
class AveragingInfo;
class PostProcessingInfo;
class PeriodicManager;
class Realms;
class Simulation;
class SolutionOptions;
class TimeIntegrator;
class MasterElement;
class PropertyEvaluator;
class HDF5FilePtr;
class Transfer;

class Realm {
public:

  Realm(Realms&);

  ~Realm();

  typedef size_t SizeType;

  void load(const YAML::Node & node);

  void breadboard();

  void initialize();
  void sample_look_ahead();

  Simulation *root() const;
  Simulation *root();
  Realms *parent() const;
  Realms *parent();

  bool debug() const;

  void create_mesh();

  void setup_adaptivity();

  void setup_nodal_fields();
  void setup_edge_fields();
  void setup_element_fields();

  void setup_interior_algorithms();
  void setup_post_processing_algorithms();
  void setup_bc();
  void enforce_bc_on_exposed_faces();
  void setup_initial_conditions();
  void setup_property();
  void extract_universal_constant( 
    const std::string name, double &value, const bool useDefault);
  void pre_timestep_work();
  void evaluate_properties();
  void augment_property_map(
    PropertyIdentifier propID,
    ScalarFieldType *theField);

  void makeSureNodesHaveValidTopology();

  void initialize_global_variables();

  void create_output_mesh();
  void create_restart_mesh();
  void input_variables_from_mesh();

  void augment_output_variable_list(
      const std::string fieldName);
  
  void augment_restart_variable_list(
      std::string restartFieldName);

  void create_edges();
  void provide_entity_count();
  void delete_edges();
  void register_fields();
  void commit();

  void process_mesh_motion();
  void init_current_coordinates();

  std::string get_coordinates_name();
  bool has_mesh_motion();
  bool has_mesh_deformation();
  bool does_mesh_move();
  bool has_non_matching_boundary_face_alg();

  void set_omega(
    stk::mesh::Part *targetPart,
    double omega);
  void set_current_displacement(
    stk::mesh::Part *targetPart);
  void set_current_coordinates(
    stk::mesh::Part *targetPart);
  void set_mesh_velocity(
    stk::mesh::Part *targetPart);

  void initialize_contact();
  void initialize_non_conformal();

  void compute_geometry();
  void compute_vrtm();
  void compute_l2_scaling();
  void advance_time_step();
  void output_converged_results();
  double compute_adaptive_time_step();
  void provide_output();
  void provide_restart_output();

  void register_interior_algorithm(
    stk::mesh::Part *part);

  void register_nodal_fields(
    stk::mesh::Part *part);

  void register_averaging_variables(
    stk::mesh::Part *part);

  void register_wall_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void register_inflow_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void register_open_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void register_contact_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo,
    const ContactBoundaryConditionData &contactBCData);

  void register_symmetry_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void register_periodic_bc(
    stk::mesh::Part *masterMeshPart,
    stk::mesh::Part *slaveMeshPart,
    const double &searchTolerance,
    const std::string &searchMethodName);

  void setup_non_conformal_bc(
    stk::mesh::Part *currentPart,
    stk::mesh::Part *opposingPart,
    const NonConformalBoundaryConditionData &nonConformalBCData);

  void register_non_conformal_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void periodic_field_update(
    stk::mesh::FieldBase *theField,
    const unsigned &sizeOfTheField,
    const bool &bypassFieldCheck = true) const;

  void periodic_delta_solution_update(
     stk::mesh::FieldBase *theField,
     const unsigned &sizeOfTheField) const;

  void periodic_max_field_update(
     stk::mesh::FieldBase *theField,
     const unsigned &sizeOfTheField) const;

  const stk::mesh::PartVector &get_slave_part_vector();

  void swap_states();

  void predict_state();

  void populate_initial_condition();
  void set_global_id();
  void populate_boundary_data();
  double populate_restart( double &timeStepNm1, int &timeStepCount);
  void populate_variables_from_input();
  void populate_derived_quantities();
  void initial_work();
  void output_banner();

  /// check job for fitting in memory
  void check_job(bool get_node_count);

  void boundary_data_to_state_data();
  void dump_simulation_time();
  double provide_mean_norm();

  MasterElement* get_surface_master_element(
    const stk::topology & theTopo);
  MasterElement* get_volume_master_element(
    const stk::topology & theTopo);

  double get_hybrid_factor(
    const std::string dofname);
  double get_alpha_factor(
    const std::string dofname);
  double get_alpha_upw_factor(
    const std::string dofname);
  double get_upw_factor(
    const std::string dofname);
  bool primitive_uses_limiter(
    const std::string dofname);
  double get_lam_schmidt(
    const std::string dofname);
  double get_lam_prandtl(
    const std::string dofname);
  double get_turb_schmidt(
    const std::string dofname);
  double get_turb_prandtl(
    const std::string dofname);
  bool get_noc_usage(
    const std::string dofname);
  double get_divU();

  // pressure poisson nuance
  double get_mdot_interp();
  bool get_cvfem_shifted_mdot();
  bool get_cvfem_shifted_poisson();
  bool get_cvfem_reduced_sens_poisson();
  
  bool has_nc_gauss_labatto_quadrature();
  NonConformalAlgType get_nc_alg_type();
  bool get_nc_alg_upwind_advection();

  PropertyEvaluator *
  get_material_prop_eval(
    const PropertyIdentifier thePropID);

  bool is_turbulent();
  void is_turbulent(
    bool isIt);

  bool needs_enthalpy();
  void needs_enthalpy(bool needsEnthalpy);

  int number_of_states();

  // redirection of stk::mesh::get_buckets to allow global selector
  //  to be applied, e.g., in adaptivity we need to avoid the parent
  //  elements
  stk::mesh::BucketVector const& get_buckets( stk::mesh::EntityRank rank,
                                              const stk::mesh::Selector & selector ,
                                              bool get_all = false) const;

  // get aura, bulk and meta data
  bool get_activate_aura();
  stk::mesh::BulkData & bulk_data();
  stk::mesh::MetaData & meta_data();

  Realms& realms_;

  std::string inputDBName_;
  unsigned spatialDimension_;

  bool realmUsesEdges_;
  int solveFrequency_;
  bool isTurbulent_;
  bool needsEnthalpy_;

  double l2Scaling_;

  // ioBroker, meta and bulk data
  stk::mesh::MetaData *metaData_;
  stk::mesh::BulkData *bulkData_;
  stk::io::StkMeshIoBroker *ioBroker_;

  size_t resultsFileIndex_;
  size_t restartFileIndex_;

  // nalu field data
  GlobalIdFieldType *naluGlobalId_;

  // algorithm drivers managed by region
  ComputeGeometryAlgorithmDriver *computeGeometryAlgDriver_;
  AlgorithmDriver *extrusionMeshDistanceAlgDriver_;
  ErrorIndicatorAlgorithmDriver *errorIndicatorAlgDriver_;
# if defined (NALU_USES_PERCEPT)  
  Adapter *adapter_;
#endif
  unsigned numInitialElements_;
  // for element, side, edge, node rank (node not used)
  stk::mesh::Selector adapterSelector_[4];
  Teuchos::RCP<stk::mesh::Selector> activePartForIO_;
  AlgorithmDriver *postConvergedAlgDriver_;
  std::vector<Algorithm *> postConvergedAlg_;

  TimeIntegrator *timeIntegrator_;

  std::string name_;

  BoundaryConditions boundaryConditions_;
  InitialConditions initialConditions_;
  MaterialPropertys materialPropertys_;

  EquationSystems equationSystems_;

  double maxCourant_;
  double maxReynolds_;
  double targetCourant_;
  double timeStepChangeFactor_;
  int currentNonlinearIteration_;

  SolutionOptions *solutionOptions_;
  OutputInfo *outputInfo_;
  AveragingInfo *averagingInfo_;
  PostProcessingInfo *postProcessingInfo_;

  std::vector<Algorithm *> propertyAlg_;
  std::map<PropertyIdentifier, ScalarFieldType *> propertyMap_;
  std::vector<Algorithm *> initCondAlg_;

  std::map<stk::topology, MasterElement *> surfaceMeMap_;
  std::map<stk::topology, MasterElement *> volumeMeMap_;

  SizeType nodeCount_;
  bool estimateMemoryOnly_;
  double availableMemoryPerCoreGB_;
  double timerReadMesh_;
  double timerOutputFields_;
  double timerCreateEdges_;
  double timerContact_;
  double timerInitializeEqs_;
  double timerPropertyEval_;
  double timerAdapt_;
  double timerTransferSearch_;

  ContactManager *contactManager_;
  NonConformalManager *nonConformalManager_;
  bool hasContact_;
  bool hasNonConformal_;
  bool hasTransfer_;

  PeriodicManager *periodicManager_;
  bool hasPeriodic_;

  // global parameter list
  stk::util::ParameterList globalParameters_;

  // part for all exposed surfaces in the mesh
  stk::mesh::Part *exposedBoundaryPart_;

  // part for new edges
  stk::mesh::Part *edgesPart_;

  bool checkForMissingBcs_;

  // types of physics
  bool isothermalFlow_;
  bool uniformFlow_;

  // some post processing of entity counts
  bool provideEntityCount_;

  // pointer to HDF5 file structure holding table
  HDF5FilePtr *HDF5ptr_;

  // automatic mesh decomposition; None, rib, rcb, multikl, etc.
  std::string autoDecompType_;

  // allow aura to be optional
  bool activateAura_;

  // mesh parts for all boundary conditions
  stk::mesh::PartVector bcPartVec_;

  // empty part vector should it be required
  stk::mesh::PartVector emptyPartVector_;

  std::vector<AuxFunctionAlgorithm *> bcDataAlg_;

  // transfer information
  std::vector<Transfer *> transferVec_;
  void augment_transfer_vector(Transfer *transfer);
  void process_transfer();

  // process end of time step converged work
  void post_converged_work();

  // time information; calls through timeIntegrator
  double get_current_time();
  double get_time_step();
  double get_gamma1();
  double get_gamma2();
  double get_gamma3();
  int get_time_step_count() const;

  // restart
  bool restarted_simulation();

  double get_stefan_boltzmann();
  double get_turb_model_constant(
    const TurbulenceModelConstant turbModelEnum);
  bool process_adaptivity();

};

} // namespace nalu
} // namespace Sierra

#endif
