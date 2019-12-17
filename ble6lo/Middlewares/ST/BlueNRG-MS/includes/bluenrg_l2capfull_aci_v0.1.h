/*
 *
 */


#ifndef _BLUENRG_L2CAP_FULL_ACI_H_
#define _BLUENRG_L2CAP_FULL_ACI_H_



#define BLE_L2CAP_SVC_BASE     0xB0       /**< L2CAP BLE SVC base. */
#define BLE_L2CAP_SVC_LAST     0xBF       /**< Total: 16. */
#define BLE_L2CAP_EVT_BASE     0x70       /**< L2CAP BLE Event base. */
#define BLE_L2CAP_EVT_LAST     0x8F       /**< Total: 32.  */

/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            6

/**@brief Bluetooth Low Energy address. */
typedef struct
{
  uint8_t addr_type;                    /**< See @ref BLE_GAP_ADDR_TYPES. */
  uint8_t addr[BLE_GAP_ADDR_LEN];       /**< 48-bit address, LSB format. */
} ble_gap_addr_t;

/**@addtogroup BLE_L2CAP_ENUMERATIONS Enumerations
 * @{ */

/**@brief L2CAP API SVC numbers. */
enum BLE_L2CAP_SVCS
{
  SD_BLE_L2CAP_CH_CONN_REQUEST = BLE_L2CAP_SVC_BASE,    /**< Send a Connection Request. */
  SD_BLE_L2CAP_CH_CONN_REPLY,                           /**< Reply to a Connection Request. */
  SD_BLE_L2CAP_CH_DISCONNECT,                           /**< Disconnect a channel. */
  SD_BLE_L2CAP_CH_TX,                                   /**< Transmit a packet. */
  SD_BLE_L2CAP_CH_RX_COMPLETE,                          /**< Notify the stack of the completion of reception. */
  SD_BLE_L2CAP_CH_RX_CREDITS_ISSUE                      /**< Issue credits. */
};

/**@brief L2CAP Event IDs. */
enum BLE_L2CAP_EVTS
{
  BLE_L2CAP_EVT_CH_CONN_REQUEST  = BLE_L2CAP_EVT_BASE,      /**< L2CAP channel connection request. */
  BLE_L2CAP_EVT_CH_CONN_REPLY,                              /**< L2CAP channel connection reply. */
  BLE_L2CAP_EVT_CH_DISCONNECTED,                            /**< L2CAP channel disconnected. */
  BLE_L2CAP_EVT_CH_RX                                       /**< L2CAP channel packet received. */
};

/** @} */


/**@addtogroup BLE_L2CAP_DEFINES Defines
 * @{ */

/**@brief Default L2CAP MTU. */
#define BLE_L2CAP_MTU_DEF           (23)

/**@brief Maximum number of connection oriented channels per physical link. */
#define BLE_L2CAP_CH_COUNT_MAX      (2)
/** @} */

/** @defgroup BLE_L2CAP_CH_CONN_REPLY_RESULTS L2CAP connection reply result codes
 * @{ */
/* L2CAP Connection Response result codes */
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_SUCCESS                (0x0000)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_PSM_NOT_SUPPORTED      (0x0002)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_NO_RESOURCES           (0x0004)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_INSUFF_AUTHENT         (0x0005)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_INSUFF_AUTHOR          (0x0006)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_INSUFF_ENC_KEY_SIZE    (0x0007)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_INSUFF_ENC             (0x0008)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_ERROR_LOC              (0xFF00)
#define BLE_L2CAP_CH_CONN_REPLY_RESULT_ERROR_REM              (0xFF01)
/** @} */

/**@addtogroup BLE_L2CAP_STRUCTURES Structures
 * @{ */

/**@brief L2CAP Channel Connection Request event. */
typedef struct
{
  uint16_t   psm;                                 /**< PSM the peer is requesting to connect to. */
  uint16_t   peer_cid;                            /**< Peer CID. */
  uint16_t   peer_mtu;                            /**< MTU the peer is able to receive. */
  ble_gap_addr_t peer_addr;                       /**< Peer Bluetooth Address */
} ble_l2cap_evt_ch_conn_request_t;

/**@brief L2CAP Channel Connection Reply event. */
typedef struct
{
  uint16_t   result;                              /**< Result code, see @ref BLE_L2CAP_CH_CONN_REPLY_RESULTS */
  uint16_t   loc_cid;                             /**< Local CID. */
  uint16_t   peer_cid;                            /**< Peer CID. */
  uint16_t   peer_mtu;                            /**< MTU the peer is able to receive. */
  uint16_t   tx_mps;                              /**< Negotiated TX MPS. */
} ble_l2cap_evt_ch_conn_reply_t;

/**@brief L2CAP Channel disconnected event report. */
typedef struct
{
  uint16_t   local_cid;                           /**< Local Channel ID for the channel that was disconnected. */
} ble_l2cap_evt_ch_disconnected_t;

/**@brief L2CAP Channel received packet event report. */
typedef struct
{
  uint16_t   local_cid;                          /**< Local Channel ID on which the packet was received. */
  uint16_t   credits;                            /**< Credits consumed by this SDU. */
  uint8_t*   p_data;                             /**< Pointer to packet data. */
  uint16_t   len;                                /**< Packet data length. */
} ble_l2cap_evt_ch_rx_t;

/**@brief L2CAP event callback event structure. */
typedef struct
{
  uint16_t conn_handle;                           /**< Connection Handle on which event occured. */
  union
  {
    ble_l2cap_evt_ch_conn_request_t     ch_conn_request;      /**< Channel Connection Request Event parameters. */
    ble_l2cap_evt_ch_conn_reply_t       ch_conn_reply;        /**< Channel Connection Reply Event parameters. */
    ble_l2cap_evt_ch_disconnected_t     ch_disconnected;      /**< Channel Disconnected Event parameters. */
    ble_l2cap_evt_ch_rx_t               ch_rx;                /**< Channel RX Event parameters. */
  } params;                                                   /**< Event Parameters. */
} ble_l2cap_evt_t;

/**@brief L2CAP connection common params structure. */
typedef struct
{
  uint16_t pub_mtu;                 /**< Public Maximum transmission unit the application is able to receive, 0 is no reception necessary, otherwise at least @ref BLE_L2CAP_MTU_DEF. */
  uint16_t act_mtu;                 /**< Actual Maximum transmission unit the application is able to receive, 0 is no reception necessary, otherwise at least @ref BLE_L2CAP_MTU_DEF. */

  uint16_t *p_tx_mps;               /**< Transmission maximum packet size, first read and final value stored */
  uint16_t *p_rx_mps;               /**< Reception maximum packet size, first read and final value stored */

  uint8_t *p_rx_mem;                /**< Pointer to the application buffer used to store received packets, NULL if no reception. */
  uint16_t rx_mem_len;              /**< Length of the application buffer, must be a multiple of act_mtu bytes. */
  uint16_t rx_creds;                /**< Initial RX credits */

} ble_l2cap_ch_conn_params_t;

/**@brief L2CAP connect request params structure. */
typedef struct
{
  uint16_t psm;                     /**< PSM to connect to. */

  ble_l2cap_ch_conn_params_t ch_conn_params; /**< Channel connection parameters */

} ble_l2cap_ch_conn_request_params_t;

/**@brief L2CAP connect reply params structure. */
typedef struct
{
  uint16_t peer_cid;                /**< Peer CID. */
  uint16_t result;                  /**< Result code, see @ref BLE_L2CAP_CH_CONN_REPLY_RESULTS */

  ble_l2cap_ch_conn_params_t ch_conn_params; /**< Channel connection parameters */

} ble_l2cap_ch_conn_reply_params_t;

/** @} */

/**@addtogroup BLE_L2CAP_FUNCTIONS Functions
 * @{ */

/**@brief Send an L2CAP channel connection request.
 *
 * @details This sends an L2CAP channel establishment request to a peer.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] p_params Connection Reply params.
 * @param[out] p_local_cid Filled in by the stack with the local CID assigned.
 *
 * @retval ::NRF_SUCCESS Successfully replied to the peer.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid State to perform operation.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 * @retval ::NRF_ERROR_BUSY SoftDevice busy, try again later.
 */
void sd_ble_l2cap_ch_conn_request(uint16_t conn_handle, ble_l2cap_ch_conn_request_params_t *p_params, uint16_t* p_local_cid);

/**@brief Reply to an L2CAP channel connection request.
 *
 * @details This replies to a peer L2CAP channel establishment request.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] p_params Connection Reply params.
 * @param[out] p_local_cid Filled in by the stack with the local CID assigned.
 *
 * @retval ::NRF_SUCCESS Successfully replied to the peer.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid State to perform operation.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 * @retval ::NRF_ERROR_BUSY SoftDevice busy, try again later.
 */
void sd_ble_l2cap_ch_conn_reply(uint16_t conn_handle, ble_l2cap_ch_conn_reply_params_t *p_params, uint16_t* p_local_cid);

/**@brief Disconnect an L2CAP channel .
 *
 * @details This issues a disconnection request to the peer.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] local_cid Local CID for the channel to be disconnected.
 *
 * @retval ::NRF_SUCCESS Successfully replied to the peer.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid State to perform operation.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 * @retval ::NRF_ERROR_BUSY SoftDevice busy, try again later.
 */
void sd_ble_l2cap_ch_disconnect(uint16_t conn_handle, uint16_t local_cid);

/**@brief Transmit an L2CAP SDU.
 *
 * @note    It is important to note that a call to this function will require the application to keep this buffer alive until the corresponding @ref BLE_EVT_TX_COMPLETE
 *          event is issued. Another call to this function may be queued before the SDU is fully transmitted as long as enough internal resources are available.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] local_cid   Local CID for the channel to use for transmission.
 * @param[in] p_data      Pointer to the data to be transmitted.
 * @param[in] len         Length of the data pointed to by p_data.
 *
 * @retval ::NRF_SUCCESS Successfully queued an L2CAP packet for transmission.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied, check against the peer MTU.
 * @retval ::NRF_ERROR_BUSY SoftDevice busy, try again later.
 */
void sd_ble_l2cap_ch_tx(uint16_t conn_handle, uint16_t local_cid, uint8_t const *p_data, uint16_t len);

/**@brief Notify the stack of the end of the processing of a received L2CAP SDU.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] local_cid   Local CID for the channel the packet was received on.
 * @param[in] p_data      Pointer to the data that was processed by the application.
 *
 * @retval ::NRF_SUCCESS Successfully queued an L2CAP packet for transmission.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 */
void sd_ble_l2cap_ch_rx_complete(uint16_t conn_handle, uint16_t local_cid, uint8_t const *p_data);

/**@brief Issue credits.
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] local_cid   Local CID for the channel the packet was received on.
 * @param[in] credits     Credits to issue.
 *
 * @retval ::NRF_SUCCESS Successfully queued an L2CAP packet for transmission.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 */
void sd_ble_l2cap_ch_rx_credits_issue(uint16_t conn_handle, uint16_t local_cid, uint16_t credits);

/** @} */

#endif // BLE_L2CAP_H__

/**
  @}
*/
