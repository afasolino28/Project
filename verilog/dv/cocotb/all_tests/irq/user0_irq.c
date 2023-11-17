/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <firmware_apis.h>




void main(){
    enable_debug();

    GPIOs_loadConfigs();
    IRQ_enableUser0(1);

    // test interrrupt happen when mprj[7] is asserted
    IRQ_clearFlag();
    set_debug_reg2(0xAA); //wait for environment to make irq[1] high 
    // Loop, waiting for the interrupt to change reg_mprj_datah
    char is_pass = 0;
    int timeout = 40; 

    for (int i = 0; i < timeout; i++){
        if (IRQ_getFlag() == 1){
            set_debug_reg1(0x1B); //test pass irq sent at mprj 7 
            is_pass = 1;
            break;
        }
    }
    if (!is_pass){
        set_debug_reg1(0x1E); // timeout
    }

    // test interrupt doesn't happened when mprj[7] is deasserted
    set_debug_reg2(0xBB);
    IRQ_enableUser0(0);
    IRQ_enableUser0(1);
    // Loop, waiting for the interrupt to change reg_mprj_datah
    is_pass = 0;

    for (int i = 0; i < timeout; i++){
        if (IRQ_getFlag() == 1){
            set_debug_reg1(0x2E); //test fail interrupt isn't suppose to happened
            is_pass = 1;
            break;
        }
    }
    if (!is_pass){
        set_debug_reg1(0x2B); // test pass
    }

    // test finish 
    set_debug_reg2(0xFF);
}
