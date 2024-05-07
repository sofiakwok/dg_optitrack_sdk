/**
 * @file optitrack_client_entity.cpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief
 * @version 0.1
 * @date 2018-12-18
 *
 * @copyright Copyright (c) 2018
 *
 */

#include <iostream>
#include "dg_optitrack_sdk/optitrack_client_entity.hpp"

using namespace std;
// using namespace dynamicgraph::sot;
using namespace dynamicgraph;

namespace dg_optitrack_sdk {

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(OptitrackClientEntity, "OptitrackClientEntity");

OptitrackClientEntity::OptitrackClientEntity( const std::string& name ):
  Entity( name ),
  // Define a signal that is always ready so the output signal is evaluated
  internal_signal_refresher_(
    "OptitrackClientEntity("+name+")::intern(dummy)::refresher" ),
  // Actual optitrack Client
  optitrack_client_(nullptr)
{
  // Define the refresh signal as always ready.
  internal_signal_refresher_.setDependencyType(
    dynamicgraph::TimeDependency<int>::ALWAYS_READY);

  // Commands:
  std::string cmd_name;
  std::string general_doc_string;
  std::string input_doc_string;

  // Commands - connect_to_optitrack
  cmd_name = "connect_to_optitrack";
  general_doc_string = "Connect to the host and start acquiring the data.";
  input_doc_string = "string: name of the host computer i.e. "
                     "[ip address]:[port number]";
  addCommand (
    cmd_name,
    dynamicgraph::command::makeCommandVoid1(
      *this,
      &OptitrackClientEntity::connect_to_optitrack,
      dynamicgraph::command::docCommandVoid1(general_doc_string,
                                             input_doc_string)
    )
  );

}

void OptitrackClientEntity::add_object_to_track(
  const std::string& optitrack_object_name)
{
  if(optitrack_client_)
  {
    // get subject name and segment name
    size_t delimiter_pos = optitrack_object_name.find("/");
    std::string subject_name = optitrack_object_name.substr(0, delimiter_pos);
    std::string segment_name = optitrack_object_name.substr(delimiter_pos+1,
                                                        optitrack_object_name.size());

    // create some temporary variables
    std::string signal_name;
    std::unique_ptr<SignalOut> signal;
    auto callback_func = boost::bind(
      &OptitrackClientEntity::signal_callbacks_position,
      this, optitrack_object_name, _1, _2); // using auto here to get the type...

    /**
     * @brief Create the position signal
     */
    signal_name = "OptitrackClientEntity("+name+")::output(vector)::" +
                  subject_name + "::" + segment_name + "_position";
    callback_func = boost::bind(
      &OptitrackClientEntity::signal_callbacks_position,
      this, optitrack_object_name, _1, _2);

    // Create the signal object
    signal.reset(new SignalOut(
      callback_func, internal_signal_refresher_, signal_name));
    signalRegistration( *signal );
    output_signal_list_.push_back(std::move(signal));

    /**
     * @brief Create the velocity world signal
     */
    signal_name = "OptitrackClientEntity("+name+")::output(vector)::" +
                  subject_name + "::" + segment_name + "_velocity_world";
    callback_func = boost::bind(
      &OptitrackClientEntity::signal_callbacks_velocity_world, this,
      optitrack_object_name, _1, _2);

    // Create the signal object
    signal.reset(
        new SignalOut(callback_func, internal_signal_refresher_, signal_name));
    signalRegistration( *signal );
    output_signal_list_.push_back(std::move(signal));

    /**
     * @brief Create the velocity body signal
     */
    signal_name = "OptitrackClientEntity("+name+")::output(vector)::" +
                  subject_name + "::" + segment_name + "_velocity_body";
    callback_func = boost::bind(
      &OptitrackClientEntity::signal_callbacks_velocity_body, this,
      optitrack_object_name, _1, _2);

    // Create the signal object
    signal.reset(
        new SignalOut(callback_func, internal_signal_refresher_, signal_name));
    signalRegistration( *signal );
    output_signal_list_.push_back(std::move(signal));
  }else{
    std::cout << "optitrackClient: request to acquire a optitrack object but the optitrack "
              << "client has not been initialized yet. Please call: "
              << "OptitrackClientEntity::connect_to_optitrack(str(host_name))."
              << std::endl;
  }
}

void OptitrackClientEntity::get_optitrack_frame(const std::string& optitrack_object_name)
{
  if(!optitrack_client_)
  {
    throw std::runtime_error("OptitrackClientEntity::get_optitrack_frame(" +
                             optitrack_object_name +
                             "): The optitrack client has not been initiliazed.");
  }
  if(!optitrack_client_->get_optitrack_frame(optitrack_object_name, optitrack_frame))
  {
    throw std::runtime_error("OptitrackClientEntity::get_optitrack_frame(" +
                             optitrack_object_name +
                             ") the required object is not streamed");
  }
}

dynamicgraph::Vector& OptitrackClientEntity::signal_callbacks_position(
  const std::string& optitrack_object_name,
  dynamicgraph::Vector& res,
  const int& /*time*/)
{
  get_optitrack_frame(optitrack_object_name);
  res = optitrack_frame.se3_pose_;
  return res;
}

dynamicgraph::Vector& OptitrackClientEntity::signal_callbacks_velocity_world(
  const std::string& optitrack_object_name,
  dynamicgraph::Vector& res,
  const int& /*time*/)
{
  get_optitrack_frame(optitrack_object_name);
  res = optitrack_frame.velocity_world_frame_;
  return res;
}

dynamicgraph::Vector& OptitrackClientEntity::signal_callbacks_velocity_body(
  const std::string& optitrack_object_name,
  dynamicgraph::Vector& res,
  const int& /*time*/)
{
  get_optitrack_frame(optitrack_object_name);
  res = optitrack_frame.velocity_body_frame_;
  return res;
}

void OptitrackClientEntity::connect_to_optitrack(const std::string& host_name)
{
  if(!optitrack_client_)
  {
    optitrack_host_name_ = host_name;
    optitrack_client_.reset(new mocap4r2_optitrack_driver::OptitrackDriverNode());
    optitrack_client_->initialize(host_name);
    optitrack_client_->run();
  }else{
    std::cout << "OptitrackClient: request to connect to: "
              << host_name
              << ". But the optitrack client is already connected to: "
              << optitrack_host_name_
              << ". Nothing to be done."
              << std::endl;
  }
}

} // namespace dg_optitrack_sdk
