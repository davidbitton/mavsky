#include <EEPROM.h>
#include "DataBroker.h"
#include "FrSkySPort.h"
#include "MavLinkData.h"
#include "MavConsole.h"

extern FrSkySPort *frsky;
extern MavLinkData *mav;
extern MavConsole *console;
extern uint16_t telem_next_extension_word();

ExtendedMessage DataBroker::extended_message_processor;
DataMap DataBroker::data_map;

DataBroker::DataBroker() {
  //mav->mavlink_sys_status_data_valid()

  if(EEPROM.read(EEPROM_ADDR_FRSKY_VFAS_ENABLE)) {
    frsky->set_vario_request_callback(get_vario_data);
  }
  frsky->set_fas_request_callback(get_fas_data);
  frsky->set_gps_request_callback(get_gps_data);
  frsky->set_rpm_request_callback(get_rpm_data);
  frsky->set_sp2uh_request_callback(get_sp2uh_data);
}

void DataBroker::write_factory_settings() {
  data_map.write_factory_settings();
  EEPROM.write(EEPROM_ADDR_FRSKY_VFAS_ENABLE, 1);
}

void DataBroker::console_map(char* p) {
  data_map.console_map(p);
}

void DataBroker::get_vario_data(uint32_t *altitude, uint32_t *vertical_speed) {
  *vertical_speed = mav->climb_rate * 100;                                   
  *altitude = data_map.get_mapped_data(data_map.VARIO_ALTITUDE);
}

void DataBroker::get_fas_data(uint32_t *voltage, uint32_t *current) {
  *voltage = mav->average_battery_voltage;
  *current = mav->average_battery_current;
}
  
void DataBroker::get_gps_data(uint32_t *lon, uint32_t *lat, uint32_t *alt, uint32_t *speed, uint32_t *heading) {
  *lon = mav->gps_longitude;
  *lat = mav->gps_latitude;
  *alt = mav->gps_altitude / 10;
  *speed = mav->gps_speed * 10;
  *heading = mav->heading * 100;            // 10000 = 100 deg   
}

void DataBroker::get_rpm_data(uint32_t *rpm) {
  *rpm = extended_message_processor.telem_next_extension_word();
  //*rpm = telem_next_extension_word();   
}

void DataBroker::get_sp2uh_data(uint32_t *fuel) {
  *fuel = mav->battery_remaining; 
}



