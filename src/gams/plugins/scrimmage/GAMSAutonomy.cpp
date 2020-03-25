
// SCRIMMAGE INCLUDES
#include <scrimmage/entity/Entity.h>
#include <scrimmage/math/State.h>
#include <scrimmage/plugin_manager/RegisterPlugin.h>

// GAMS INCLUDES
#include <gams/plugins/scrimmage/GAMSAutonomy.h>

// CAR CONTROLLER PLUGIN INCLUDES
#include <scrimmage/common/VariableIO.h>

#include <iostream>

REGISTER_PLUGIN(scrimmage::Autonomy,
                scrimmage::autonomy::GAMSAutonomy,
                GAMSAutonomy_plugin)

namespace scrimmage {
namespace autonomy {

GAMSAutonomy::GAMSAutonomy() {
}

/*
   @param params Stores xml params used to initialize plugin, found in gams/platforms/scrimmage/*.xml

*/
void GAMSAutonomy::init(std::map<std::string, std::string> &params) {
    position_x_idx_ = vars_.declare("position_x", scrimmage::VariableIO::Direction::Out);
    position_y_idx_ = vars_.declare("position_y", scrimmage::VariableIO::Direction::Out);
    position_z_idx_ = vars_.declare("position_z", scrimmage::VariableIO::Direction::Out);
}

/*
   @param t  time
   @param dt time interval
   @effect Changes the entities current state and desired state
*/
bool GAMSAutonomy::step_autonomy(double t, double dt) {

    // Take desired state and set to outputs
    if (this->desired_state_)
    {
        vars_.output(position_x_idx_, this->desired_state_->pos()[0]);
        vars_.output(position_y_idx_, this->desired_state_->pos()[1]);
        vars_.output(position_z_idx_, this->desired_state_->pos()[2]);
        std::cout << "Moving towards objective" << std::endl;
        
    } else 
    {
       std::cout << "WARNING: Desired state not set yet by GAMS Algorithms" << std::endl;
    }
    
    return true;
}

/*
    Spawns the entity that this plugin is attached to
*/
void GAMSAutonomy::spawn_entity(State & origin)
{
  this->pub_gen_ents_ = advertise("GlobalNetwork", "GenerateEntity");

  std::cout << "Here" << std::endl;
  // Create the GenerateEntity message
  auto msg = std::make_shared<Message<scrimmage_msgs::GenerateEntity>>();

  std::cout << "Here" << std::endl;
  // Copy the new state into the message
  scrimmage::set(msg->data.mutable_state(), origin);

  // Set the entity_tag that references the entity to be generated in the
  // mission XML file.
  
  std::cout << "Here" << std::endl;
  
  // Get entity tag from ent?
  msg->data.set_entity_tag("gen_straight");
  
  std::cout << "Here" << std::endl;

  // Publish the GenerateEntity message
  this->pub_gen_ents_->publish(msg);
}

} // ns scrimmage
} // ns autonomy
