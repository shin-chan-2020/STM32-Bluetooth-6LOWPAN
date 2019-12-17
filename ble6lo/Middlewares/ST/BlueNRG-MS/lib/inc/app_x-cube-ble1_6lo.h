/**
  ******************************************************************************
  * File Name          : app_x-cube-ble1.h
  * Description        : Header file
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
/******************************************************************************
* @file    app_x-cube-ble1_6lo.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_X_CUBE_BLE1_6LOWPAN_H
#define __APP_X_CUBE_BLE1_6LOWPAN_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f4_nucleo_f401re.h"

/* System States:SS */
typedef enum
{
  SS_NO_KNOWN = 0U,       /* not known or defined state*/
  SS_LW_INIT_START,       /* statr lwip stack initilization */
  SS_BT_DISCOVERABLE,     /* make bt discoverable to 6lowpan border router*/
  SS_BT_CONNECTED,        /* bt rf connected */
  SS_L2_WAITCON,          /* wait for l2cap connection */
  SS_L2_CONNECTED,        /* l2cap connected */
  SS_L2_CHANNEL_ON,       /* l2cap channel ready */
  SS_L2_UPDATE_REQ,       /* l2cap channel update request received*/
  SS_L2_UPDATE_RES,       /* l2cap channel update response sent*/
  SS_L2_CHANNEL_READ,     /* received data in l2cap channel, read them*/
  SS_L2_CHANNEL_PROCESS,  /* process data received in l2cap channel*/
  SS_L2_CHANNEL_WRITE,    /* write data to l2cap channel*/
  SS_ST_RESET             /* call to reset system*/
} system_states;

typedef enum
{
  L2CAP_STATE_0,
  L2CAP_STATE_1,
  L2CAP_STATE_2,
  L2CAP_STATE_3,
  L2CAP_STATE_4
}l2cap_states;

/* Exported Functions --------------------------------------------------------*/
void MX_X_CUBE_BLE1_Init(void);
void MX_X_CUBE_BLE1_Process(void);
uint8_t is_acl_buufer_available(void);

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_6LOWPAN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
