#ifndef CAN_EXT
#define CAN_EXT

#define tempID 0x1839F380

void CANSetup();
void sendCANTemp(uint8_t min_temp, uint8_t max_temp, uint8_t ave_temp, uint8_t error);
void setMessageBuffer(uint8_t *data, uint8_t len);
void sendMessage(int id, uint8_t ext);

#endif