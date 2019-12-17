/**
******************************************************************************
* File Name          : app_x-cube-ble1.c
* Description        : Implementation file
*
******************************************************************************
*
* COPYRIGHT 2018 STMicroelectronics
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
******************************************************************************
*/
/**
******************************************************************************
* @file    app_x-cube-ble1_6lo.c
* @author  xxx.jls@gmail.com
* @version V1.0.0
* @date    04-Dec-2019
* @brief   This code is modified by author xxx.jls@gmail.com from original code.
           The author holds the exclusive COPYRIGHT(c) to the modified part.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2019 xxx.jls@gmail.com</center></h2>
*
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* USER CODE BEGIN Includes */
/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-ble1_6lo.h"
#include "sample_service_6lo.h"
#include "hci_tl.h"
#include "role_type.h"
#include "bluenrg_utils.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include "netif/lowpan6_common.h"
/* USER CODE END Includes */


/*
Internet Protocol Support Profile
Revision V1.0.0
Page: 9

A device supporting the Node role shall implement the GATT server role and
instantiate one and only one IP Support Service (IPSS).
*/
/* Private defines -----------------------------------------------------------*/
#define GO_TO_SS(state) {g_system_state = state;}
/* Private macros ------------------------------------------------------------*/
#define SERVER_ROLE
#define BDADDR_SIZE 6
#define MILLISEC_1000 1000
#define MILLISEC_5000 5000

/* Private variables ---------------------------------------------------------*/
static system_states g_system_state = SS_NO_KNOWN;
/* Global variables ---------------------------------------------------------*/
l2cap_states g_l2cap_state = L2CAP_STATE_0;
uint8_t      g_ipv6_up = FALSE;
uint8_t      g_bnrg_expansion_board = IDB05A1; /* at startup, suppose the X-NUCLEO-IDB05A1 is used */
uint8_t      g_bdaddr[BDADDR_SIZE];
tBDAddr      g_brouter_bdaddr;
uint16_t     g_data_read = 0;

#ifdef SERVER_ROLE
BLE_RoleTypeDef BLE_Role = SERVER;
#else
BLE_RoleTypeDef BLE_Role = CLIENT;
#endif

const char *google_ping_ipv6_addr = "2404:6800:4002:802::200e";

/*
A static address is a 48-bit randomly generated address and shall meet the following
requirements:

BLUETOOTH SPECIFICATION Version 4.2 [Vol 6, Part B] page 33
LE device has a different address format than other devices
*/
/* Public Address */
/* 00:80:E1:00:00:AA */
/* uint8_t SERVER_BDADDR[] = {0xAA, 0x00, 0x00, 0xE1, 0x80, 0x00}; */
/* Local Address */
/* 02:80:E1:00:00:AA */
uint8_t SERVER_BDADDR[] = {0xAA, 0x00, 0x00, 0xE1, 0x80, 0x02};

extern volatile uint8_t   set_connectable;
extern volatile uint8_t   g_bt_connected;
extern          uint16_t  Remote_CID;
extern struct   lowpan6_link_addr rfc7668_peer_addr;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void User_Process(void);
void User_Init(void);
void ping_google(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
* @brief  This function is a utility to print the log time
*         in the format HH:MM:SS:MSS (DK GUI time format)
* @param  None
* @retval None
*/
void print_csv_time(void){
  uint32_t ms = HAL_GetTick();
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}
#endif

void MX_X_CUBE_BLE1_Init(void)
{
  /* USER CODE BEGIN SV */
  uint8_t  Host_off = 0x01;
  uint8_t  hwVersion;
  uint16_t fwVersion;
  uint8_t  ret;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  /* USER CODE END SV */

  /* USER CODE BEGIN X_CUBE_BLE1_Init_PreTreatment */

  /* USER CODE END X_CUBE_BLE1_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */


  User_Init();

  hci_init(user_notify, //this function is called whenever there is an ACI event to be processed
           NULL);

  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);

  /*
  * Reset BlueNRG again otherwise we won't
  * be able to change its MAC address.
  * aci_hal_write_config_data() must be the first
  * command after reset otherwise it will fail.
  */
  hci_reset();

  printf("HWver %d, FWver %d\n", hwVersion, fwVersion);

  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    g_bnrg_expansion_board = IDB05A1;
  }

#ifdef BLE_ADDR_RANDOM_GENAUTO
  /* we will let the BLE chip to use its Random MAC address */
  uint8_t data_len_out;
  ret = aci_hal_read_config_data(CONFIG_DATA_RANDOM_ADDRESS, 6, &data_len_out, g_bdaddr);

  if(ret){
    PRINTF("\r\nReading  Random BD_ADDR failed\r\n");
    goto fail;
  }

#else  /* BLE_ADDR_RANDOM_GENAUTO */

  if (BLE_Role == SERVER) {
    BLUENRG_memcpy(g_bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR)); /* Device address being used*/
  }
#endif /* BLE_ADDR_RANDOM_GENAUTO */
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  g_bdaddr);
  if (ret) {
    PRINTF("Setting BD_ADDR failed 0x%02x.\n", ret);
  }

  ret = aci_hal_write_config_data(CONFIG_DATA_LL_WITHOUT_HOST,
                                  CONFIG_DATA_LL_WITHOUT_HOST_LEN,
                                  &Host_off);
  if (ret) {
    PRINTF("Set to 1 to disable Host failed 0x%02x.\n", ret);
  }

  uint16_t pkt_len = 0;
  uint8_t  max_pkt = 0;

  ret = hci_le_read_buffer_size(&pkt_len, &max_pkt);
  if (ret) {
    PRINTF("hci_le_read_buffer_size 0x%02x.\n", ret);
  }
  else
  {
    //PRINTF("pkt_len = %d, max_pkt = %d\n",pkt_len,max_pkt);
  }


  ret = aci_gatt_init();
  if (ret) {
    PRINTF("GATT_Init failed.\n");
  }

  /* Register the GAP service with the GATT. */
  /* Our device is GAP Peripheral and GATT Server */

  if (BLE_Role == SERVER) {
    if (g_bnrg_expansion_board == IDB05A1) {
      ret = aci_gap_init_IDB05A1(
                                 GAP_PERIPHERAL_ROLE_IDB05A1, /* role Bitmap of allowed roles: see @ref gap_roles "GAP roles". */
                                 0,                           /* privacy_enabled Enable (1) or disable (0) privacy. */
                                 0x07,                        /* device_name_char_len Length of the device name characteristic */
                                 &service_handle,             /* [out]  service_handle  Handle of the GAP service. */
                                 &dev_name_char_handle,       /* dev_name_char_handle  Device Name Characteristic handle */
                                 &appearance_char_handle);    /* appearance_char_handle Appearance Characteristic handle */
    }

  }


  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("GAP_Init failed.\n");
  }

  PRINTF("GAP service_handle = 0x%X dev_name_char_handle = 0x%X appearance_char_handle = 0x%X\n",
         service_handle,        dev_name_char_handle,       appearance_char_handle );

  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_NOT_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     DONOT_USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     NO_BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    printf("\nBLE Stack Initialized.\n\n");
  }

  if (BLE_Role == SERVER) {
    PRINTF("SERVER: BLE Stack Initialized\n");
    ret = Add_Sample_Service();

    if (ret == BLE_STATUS_SUCCESS)
      PRINTF("Service added successfully.\n");
    else
      PRINTF("Error while adding service.\n");

  } else {
    PRINTF("CLIENT: BLE Stack Initialized\n");
  }

  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);

  /* USER CODE BEGIN X_CUBE_BLE1_Init_PostTreatment */
fail:
  return;
  /* USER CODE END X_CUBE_BLE1_Init_PostTreatment */
}

/**
* @brief  Initialize User process.
*
* @param  None
* @retval None
*/
void User_Init(void)
{
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED2);
  BSP_COM_Init(COM1);
}


/*
* BlueNRG-MS background task
*/
void MX_X_CUBE_BLE1_Process(void)
{

  User_Process();

}



/**
* @brief  Configure the device as Client or Server and manage the communication
*         between a client and a server.
*
* @param  None
* @retval None
*/


static void User_Process(void){

  uint8_t send_data[] = "Hello World!";
  static uint8_t waiting_recv = FALSE;
  uint8_t ubutton_state;
  ip_addr_t    brouter_lladdr;


  switch(g_system_state)
  {
  case SS_NO_KNOWN:

    GO_TO_SS(SS_LW_INIT_START);
    break;
  case SS_LW_INIT_START:

    lwip_main();

    GO_TO_SS(SS_BT_DISCOVERABLE);
    break;
  case SS_BT_DISCOVERABLE:

    if (set_connectable)
    {
      /* Use last 16 bits of BT address in name of the BT device */
      int16_t name_ext = (g_bdaddr[4] << 8) | g_bdaddr[5];
      /* Establish connection with remote device */
      Make_Connection(name_ext);
      set_connectable = FALSE;
      ubutton_state = BSP_PB_GetState(BUTTON_KEY);

      printf("\n... this BT device now is ready for connection from Border Router\n\n");
      printf("from Border Router connect to Bluetooth @ address => ");

      for(int i = 5; i >= 0 ; i--)
      {
        printf("%X:", g_bdaddr[i]);
      }
      printf("\n\n");

      GO_TO_SS(SS_BT_CONNECTED);
    }
    break;
  case SS_BT_CONNECTED:

    if(g_bt_connected)
    {
      printf("Bluetooth connected...IPv6 connection being established...wait...\n\n");

      GO_TO_SS(SS_L2_WAITCON);
    }
    else
    {
      //it will go out of this case and call hci_user_evt_proc()
    }
    break;
  case SS_L2_WAITCON:

    if(g_l2cap_state == L2CAP_STATE_2)
    {
      HAL_Delay(MILLISEC_1000);
      GO_TO_SS(SS_L2_CONNECTED);
    }
    else if(g_l2cap_state == L2CAP_STATE_3)
    {
      GO_TO_SS(SS_L2_CHANNEL_ON);
    }
    break;
  case SS_L2_CONNECTED:

    if(g_l2cap_state == L2CAP_STATE_3)
    {
      GO_TO_SS(SS_L2_CHANNEL_ON);
    }
    break;
  case SS_L2_CHANNEL_ON:

    peer_btadder_to_llip6addr();

    printf("*******************************************************************\n");
    printf("%s","\nA 6LoWPAN Border Router (LBR) located @ ble addr: ");
    for(int i = 0; i < sizeof(tBDAddr); i++)
    {
      printf("%X:", g_brouter_bdaddr[i]);
    }
    printf("\b \n");
    printf("*******************************************************************\n");

    g_ipv6_up = TRUE;

    peer_link_addr_to_ip6addr(&brouter_lladdr, &rfc7668_peer_addr);
    printf("\nA 6LoWPAN Border Router (LBR) located @ IPv6 Link Local Addr: %s\n",ip6addr_ntoa(&brouter_lladdr));
    printf("*******************************************************************\n");

    GO_TO_SS(SS_L2_CHANNEL_READ);
    break;
  case SS_L2_CHANNEL_READ:
    /* Read a received packet from the BT buffers and send it
    to the lwIP for handling. same as ethernetif_input(&gnetif) */

    if(g_data_read){
      waiting_recv = FALSE;
      mch_net_poll();
    }
    else
    {
      break;
    }
  case SS_L2_CHANNEL_PROCESS:

    static uint8_t ping_requested = 0;

    if(!ping_requested){
      printf("\nPress the BLUE user button within the next 5 seconds to ping router...\n");

      if (Button_WaitForPush(MILLISEC_5000))
      {
        tcp_echoclient_connect(&brouter_lladdr);
        /*ping_google();*/
        ping_requested = 1;
      }
    }

    hci_user_evt_proc();

    g_l2cap_state = L2CAP_STATE_4;

    GO_TO_SS(SS_L2_CHANNEL_READ);

    break;
  case SS_L2_CHANNEL_WRITE:

    netif_send_bluenrg(Remote_CID, sizeof(send_data), send_data); /* use sizeof to send string null*/

    GO_TO_SS(SS_L2_CHANNEL_PROCESS);

    waiting_recv = TRUE;
    break;
  case SS_ST_RESET:

    break;
  default:
  }

}

void ping_google(void)
{
  ip6_addr_t  addr6_google;

  memset(&addr6_google, 0, sizeof(addr6_google));

  if(!ip6addr_aton(google_ping_ipv6_addr, &addr6_google))
  {
    PRINTF("wrong address format\n");
  }
  else
  {
    PRINTF("ping target: %s\n",ip6addr_ntoa(&addr6_google));
  }

  tcp_echoclient_connect(&addr6_google);

}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
