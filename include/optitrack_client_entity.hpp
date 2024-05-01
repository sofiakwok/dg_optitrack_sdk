/**
 * @file optitrack_entity.hh
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-12-18
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef OPTITRACK_CLIENT_ENTITY_HPP
#define OPTITRACK_CLIENT_ENTITY_HPP

#include <deque>

#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/all-commands.h>
#include <dynamic-graph/all-signals.h>
#include <dynamic-graph/linear-algebra.h>

#include "mocap4r2_optitrack_driver/mocap4r2_optitrack_driver.hpp"

/**
 * @brief this is this package namespace in order to avoid naming conflict
 */
namespace dg_optitrack_sdk {

/**
 * @brief Simple shortcut for code writing convenience
 */
typedef dynamicgraph::SignalTimeDependent<int,int> SignalRefresher;
/**
 * @brief Simple shortcut for code writing convenience
 */
typedef dynamicgraph::SignalTimeDependent<dynamicgraph::Vector,int> SignalOut;

/**
 * @brief This class define a dynamic graph wrapper around the optitrack client
 */
class OptitrackClientEntity: public dynamicgraph::Entity
{
public:
  /**
   * @brief The class name used to identify it in the dynamic graph pool
   */
  static const std::string CLASS_NAME;

  /**
   * @brief Get the CLASS_NAME object.
   *
   * @return const std::string&
   */
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }

  /**
   * @brief Construct a new optitrack Client Entity object.
   * 
   * @param name Entity name.
   */
  OptitrackClientEntity( const std::string& name );

  /**
   * @brief Destroy the optitrack Client Entity object.
   */
  ~OptitrackClientEntity( void ){}

  /**
   * @brief Connect to the optitrack system and start a
   *
   * @param host_name
   */
  void connect_to_optitrack(const std::string& host_name);

  /**
   * @brief Add a signal that contains the pose of the desired object.
   */
  void add_object_to_track(const std::string& object_name);

  /**
   * @brief Signal callback for position functions.
   */
  dynamicgraph::Vector& signal_callbacks_position(
      const std::string& optitrack_object_name, dynamicgraph::Vector& res,
      const int& /*time*/);

  /**
   * @brief Signal callback for velocity_world functions.
   */
  dynamicgraph::Vector& signal_callbacks_velocity_world(
      const std::string& optitrack_object_name, dynamicgraph::Vector& res,
      const int& /*time*/);

  /**
   * @brief Signal callback for velocity_body functions.
   */
  dynamicgraph::Vector& signal_callbacks_velocity_body(
      const std::string& optitrack_object_name, dynamicgraph::Vector& res,
      const int& /*time*/);

private:
  /**
   * @brief Get the optitrack frame by the provided object name.
   *
   * @param optitrack_object_name Name of the object to read last optitrack frame for.
   */
  void get_optitrack_frame(const std::string& optitrack_object_name);

  /**
   * @brief Create an internal output signal which is "ALWAYS_READY", this means
   * that the signals that depends on it will always be evaluated using the
   * callback functino provided.
   */
  SignalRefresher internal_signal_refresher_;

  /**
   * @brief This is the list of output signals registered for this class. It
   * correspond to the list of optitrack object to track. Form python call
   * add_object_to_track("[subject_name]/[segment_name]") to add such a signal.
   */
  std::deque<std::unique_ptr<SignalOut> > output_signal_list_;

  /**
   * @brief This is the optitrack client object to access to the network.
   */
  std::unique_ptr<optitrack_sdk::OptitrackClient> optitrack_client_;

  /**
   * @brief this the optitrack host name to connect to.
   */
  std::string optitrack_host_name_;
};

} // namespace dg_optitrack_sdk

#endif // optitrack_CLIENT_ENTITY_HPP