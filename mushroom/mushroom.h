

#ifndef BLE_MUSHROOM_H__
#define BLE_MUSHROOM_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"


 


typedef enum
{
    BLE_mushroom_EVT_NOTIFICATION_ENABLED,                  
    BLE_mushroom_EVT_NOTIFICATION_DISABLED                   
} ble_mushroom_evt_type_t;


typedef struct
{
    ble_mushroom_evt_type_t evt_type;                       
} ble_mushroom_evt_t;


typedef struct ble_mushroom_s ble_mushroom_t;


typedef void (*ble_mushroom_evt_handler_t) (ble_mushroom_t * p_mushroom, ble_mushroom_evt_t * p_evt);


typedef struct
{
    ble_mushroom_evt_handler_t        evt_handler;                                          /**< Event handler to be called for handling events in the Heart Rate Service. */
    ble_srv_cccd_security_mode_t      mushroom_distance_attr_md;                                      /**< Initial security level for heart rate service measurement attribute */
    ble_srv_cccd_security_mode_t      mushroom_weight_attr_md;                                      /**< Initial security level for body sensor location attribute */
} ble_mushroom_init_t;



struct ble_mushroom_s
{
	
    ble_mushroom_evt_handler_t   evt_handler;                                          /**< Event handler to be called for handling events in the Heart Rate Service. */
    uint16_t                     service_handle;                                       /**< Handle of Heart Rate Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t     distance_handles;                                          /**< Handles related to the Heart Rate Measurement characteristic. */
    ble_gatts_char_handles_t     weight_handles;                                          /**< Handles related to the Body Sensor Location characteristic. */
    uint16_t                     conn_handle;                                          /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
     bool                     is_distance_NTFenabled;                                     /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
    bool                      is_weight_NTFenabled;                                       /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/                         
};


uint32_t ble_mushroom_init(ble_mushroom_t * p_mushroom, const ble_mushroom_init_t * p_mushroom_init);


void ble_mushroom_on_ble_evt(ble_mushroom_t * p_mushroom, ble_evt_t * p_ble_evt);


uint32_t ble_mushroom_string_send(ble_mushroom_t * p_mushroom, uint8_t * p_string, uint16_t length, uint16_t handle);


#endif 


