

#include "mushroom.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"





#define OPCODE_LENGTH 1                  
#define HANDLE_LENGTH 2 
#define MAX_MUSHROOM_LEN   (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH)  





static void on_connect(ble_mushroom_t * p_mushroom, ble_evt_t * p_ble_evt)
{
    p_mushroom->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}



static void on_disconnect(ble_mushroom_t * p_mushroom, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_mushroom->conn_handle = BLE_CONN_HANDLE_INVALID;
}


//static void on_distance_cccd_write(ble_mushroom_t * p_mushroom, ble_gatts_evt_write_t * p_evt_write)
//{
//    if (p_evt_write->len == 2)
//    {
//        
//        if (p_mushroom->evt_handler != NULL)
//        {
//            ble_mushroom_evt_t evt;

//            if (ble_srv_is_notification_enabled(p_evt_write->data))
//            {
//                evt.evt_type = BLE_mushroom_EVT_NOTIFICATION_ENABLED;
//            }
//            else
//            {
//                evt.evt_type = BLE_mushroom_EVT_NOTIFICATION_DISABLED;
//            }

//            p_mushroom->evt_handler(p_mushroom, &evt);
//        }
//    }
//}


static void on_write(ble_mushroom_t * p_mushroom, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (
        (p_evt_write->handle == p_mushroom->distance_handles.cccd_handle)
        &&
        (p_evt_write->len == 2)
       )
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            p_mushroom->is_distance_NTFenabled = true;
        }
        else
        {
            p_mushroom->is_distance_NTFenabled = false;
        }
    }
    if (
        (p_evt_write->handle == p_mushroom->weight_handles.cccd_handle)
        &&
        (p_evt_write->len == 2)
       )
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            p_mushroom->is_weight_NTFenabled = true;
        }
        else
        {
            p_mushroom->is_weight_NTFenabled = false;
        }
    }
    
   

//	  if (
//             (p_evt_write->handle == p_nus->tx_handles.value_handle)
//             &&
//             (p_nus->data_handler != NULL)
//            )
//    {
//        p_nus->data_handler(p_nus, p_evt_write->data, p_evt_write->len);
//    }
//    else
//    {
//        // Do Nothing. This event is not relevant for this service.
//    }
}


void ble_mushroom_on_ble_evt(ble_mushroom_t * p_mushroom, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_mushroom, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_mushroom, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_mushroom, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}





static uint32_t distance_char_add(ble_mushroom_t            * p_mushroom,
                                                const ble_mushroom_init_t * p_mushroom_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.write_perm = p_mushroom_init->mushroom_distance_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    BLE_UUID_BLE_ASSIGN(ble_uuid, 0x0001);

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_mushroom_init->mushroom_distance_attr_md.read_perm;
    attr_md.write_perm = p_mushroom_init->mushroom_distance_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = MAX_MUSHROOM_LEN;

    return sd_ble_gatts_characteristic_add(p_mushroom->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_mushroom->distance_handles);
}



static uint32_t weight_char_add(ble_mushroom_t            * p_mushroom,
                                                const ble_mushroom_init_t * p_mushroom_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.write_perm = p_mushroom_init->mushroom_weight_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    BLE_UUID_BLE_ASSIGN(ble_uuid, 0x0002);

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_mushroom_init->mushroom_weight_attr_md.read_perm;
    attr_md.write_perm = p_mushroom_init->mushroom_weight_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = MAX_MUSHROOM_LEN;

    return sd_ble_gatts_characteristic_add(p_mushroom->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_mushroom->weight_handles);
}






uint32_t ble_mushroom_init(ble_mushroom_t * p_mushroom, const ble_mushroom_init_t * p_mushroom_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    p_mushroom->evt_handler                 = p_mushroom_init->evt_handler;

    p_mushroom->conn_handle                 = BLE_CONN_HANDLE_INVALID;
    p_mushroom->is_distance_NTFenabled = false;
    p_mushroom->is_weight_NTFenabled = false;

    BLE_UUID_BLE_ASSIGN(ble_uuid, 0X1777);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_mushroom->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = distance_char_add(p_mushroom, p_mushroom_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
		err_code = weight_char_add(p_mushroom, p_mushroom_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }


    return NRF_SUCCESS;
}



uint32_t ble_mushroom_string_send(ble_mushroom_t * p_mushroom, uint8_t * p_string, uint16_t length, uint16_t handle)
{
    ble_gatts_hvx_params_t hvx_params;

    if (p_mushroom == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (p_mushroom->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }

		
			if ( (handle == p_mushroom->distance_handles.value_handle) && (!p_mushroom->is_distance_NTFenabled)){
		return NRF_ERROR_INVALID_STATE;
	}
	if ( (handle == p_mushroom->weight_handles.value_handle) && (!p_mushroom->is_weight_NTFenabled)){
		return NRF_ERROR_INVALID_STATE;
	}

	
    if (length > MAX_MUSHROOM_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = handle;
    hvx_params.p_data = p_string;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_mushroom->conn_handle, &hvx_params);
}













