/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : bluenrg_hci.c
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 4-Oct-2013
* Description        : File with HCI commands for BlueNRG FW6.0 and above.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "bluenrg_types.h"
#include "bluenrg_def.h"
#include "hci_const.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_l2cap_aci.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_gap.h"

#define MIN(a,b)            ((a) < (b) )? (a) : (b)
#define MAX(a,b)            ((a) > (b) )? (a) : (b)

/*
Connection interval (interval_min/interval_max): Determines how often the Central will ask for data from the Peripheral.
When the Peripheral requests an update, it supplies a maximum and a minimum wanted interval. The connection interval must
be between 7.5 ms and 4 s.
Slave latency (slave_latency): By setting a non-zero slave latency, the Peripheral can choose to not answer when the Central
asks for data up to the slave latency number of times. However, if the Peripheral has data to send, it can choose to send
data at any time. This enables a peripheral to stay sleeping for a longer time, if it doesn't have data to send,
but still send data fast if needed. The text book example of such device is for example keyboard and mice, which want
to be sleeping for as long as possible when there is no data to send, but still have low latency (and for the mouse:
low connection interval) when needed.
Connection supervision timeout (timeout_multiplier): This timeout determines the timeout from the last data exchange till a
link is considered lost. A Central will not start trying to reconnect before the timeout has passed, so if you have a device
which goes in and out of range often, and you need to notice when that happens, it might make sense to have a short timeout.

The data fields are:
• Interval Min (2 octets) : Defines minimum value for the connection event interval in the following manner:
connIntervalMin = Interval Min * 1.25 ms. Interval Min range: 6 to 3200 frames
where 1 frame is 1.25 ms and equivalent to 2 BR/EDR slots. Values outside the
range are reserved. Interval Min shall be less than or equal to Interval Max.
• Interval Max (2 octets) : Defines maximum value for the connection event interval in the following manner:
connIntervalMax = Interval Max * 1.25 ms. Interval Max range: 6 to 3200
frames. Values outside the range are reserved. Interval Max shall be equal
to or greater than the Interval Min.
• Slave Latency (2 octets) : Defines the slave latency parameter (as number of LL connection events) in the following manner:
connSlaveLatency = Slave Latency. The Slave Latency field shall have a value
in the range of 0 to ((connSupervisionTimeout / (connIntervalMax*2)) -1).
The Slave Latency field shall be less than 500.
• Timeout Multiplier (2 octets) : Defines connection timeout parameter in the following manner:
connSupervisionTimeout = Timeout Multiplier * 10 ms
The Timeout Multiplier field shall have a value in the range of 10 to 3200.

ex:
connIntervalMax = 3200
Timeout Multiplier = 3200
connSupervisionTimeout = Timeout Multiplier * 10 ms = 3200 * 10 = 32000
connSlaveLatency = ((connSupervisionTimeout / (connIntervalMax*2)) -1) = ((32000/(3200*2)) -1)=4
*/
tBleStatus aci_l2cap_connection_parameter_update_request(uint16_t conn_handle, uint16_t interval_min,
							 uint16_t interval_max, uint16_t slave_latency,
							 uint16_t timeout_multiplier)
{
  struct hci_request rq;
  uint8_t status;
  l2cap_conn_param_update_req_cp cp;

  cp.conn_handle = htobs(conn_handle);
  cp.interval_min = htobs(interval_min);
  cp.interval_max = htobs(interval_max);
  cp.slave_latency = htobs(slave_latency);
  cp.timeout_multiplier = htobs(timeout_multiplier);

  BLUENRG_memset(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_L2CAP_CONN_PARAM_UPDATE_REQ;
  rq.cparam = &cp;
  rq.clen = L2CAP_CONN_PARAM_UPDATE_REQ_CP_SIZE;
  rq.event = EVT_CMD_STATUS;
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_l2cap_connection_parameter_update_response_IDB05A1(uint16_t conn_handle, uint16_t interval_min,
							 uint16_t interval_max, uint16_t slave_latency,
							 uint16_t timeout_multiplier, uint16_t min_ce_length, uint16_t max_ce_length,
                             uint8_t id, uint8_t accept)
{
  struct hci_request rq;
  uint8_t status;
  l2cap_conn_param_update_resp_cp_IDB05A1 cp;

  cp.conn_handle = htobs(conn_handle);
  cp.interval_min = htobs(interval_min);
  cp.interval_max = htobs(interval_max);
  cp.slave_latency = htobs(slave_latency);
  cp.timeout_multiplier = htobs(timeout_multiplier);
  cp.min_ce_length = htobs(min_ce_length);
  cp.max_ce_length = htobs(max_ce_length);
  cp.id = id;
  cp.accept = accept;

  BLUENRG_memset(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_L2CAP_CONN_PARAM_UPDATE_RESP;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}

tBleStatus aci_l2cap_connection_parameter_update_response_IDB04A1(uint16_t conn_handle, uint16_t interval_min,
							 uint16_t interval_max, uint16_t slave_latency,
							 uint16_t timeout_multiplier, uint8_t id, uint8_t accept)
{
  struct hci_request rq;
  uint8_t status;
  l2cap_conn_param_update_resp_cp_IDB04A1 cp;

  cp.conn_handle = htobs(conn_handle);
  cp.interval_min = htobs(interval_min);
  cp.interval_max = htobs(interval_max);
  cp.slave_latency = htobs(slave_latency);
  cp.timeout_multiplier = htobs(timeout_multiplier);
  cp.id = id;
  cp.accept = accept;

  BLUENRG_memset(&rq, 0, sizeof(rq));
  rq.ogf = OGF_VENDOR_CMD;
  rq.ocf = OCF_L2CAP_CONN_PARAM_UPDATE_RESP;
  rq.cparam = &cp;
  rq.clen = sizeof(cp);
  rq.rparam = &status;
  rq.rlen = 1;

  if (hci_send_req(&rq, FALSE) < 0)
    return BLE_STATUS_TIMEOUT;

  return status;
}
