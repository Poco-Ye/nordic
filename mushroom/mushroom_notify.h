#ifndef MUSHROOM_NOTIFY_H__
#define MUSHROOM_NOTIFY_H__

#include "sci2a.h"

typedef const struct mushroom_notify_pfn
{
   void (*__weight_handler)(ble_mushroom_t * p_mushroom); 
	 void (*__distance_handler)(ble_mushroom_t * p_mushroom,sci2a_handle * p_sci2a_handle);
	 void (*__temperature_handler)(ble_mushroom_t * p_mushroom);
}mushroom_notify_pfn;
	
	
typedef struct mushroom_handle
{
   mushroom_notify_pfn *pfn; 

}mushroom_handle;


mushroom_handle * mushrooom_init(void);




#endif

