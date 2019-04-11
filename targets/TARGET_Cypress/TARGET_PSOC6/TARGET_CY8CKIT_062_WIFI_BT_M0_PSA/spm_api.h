/* Copyright (c) 2017-2018 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SPM_API_H
#define SPM_API_H

/* ------------------------ Platform's Functions Implementation ---------------------- */
void ipc_interrupt_handler(void);
void mailbox_init(void);

/* -------------------------------------- HAL API ------------------------------------ */
void spm_hal_start_nspe(void);
void spm_hal_mailbox_notify(void);
void spm_hal_memory_protection_init(void);


#endif  /* SPM_API_H */
