/* Copyright (c) 2017-2019 ARM Limited
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

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_DEFS_INC__
#define __TFM_PARTITION_DEFS_INC__

/*************************** Service Partitions ******************************/

{% for partition in service_partitions %}
{% set partition_loop = loop %}
#define {{partition.name|upper}}_ID (TFM_SP_BASE + {{ partition_loop.index0 }})
{% endfor %}

/*************************** Test Partitions *********************************/

#ifdef USE_PSA_TEST_PARTITIONS

{% for partition in test_partitions %}
{% set partition_loop = loop %}
#ifdef USE_{{partition.name|upper}}
#define {{partition.name|upper}}_ID (TFM_SP_BASE + {{service_partitions|count}} + {{ partition_loop.index0 }})
#endif

{% endfor %}
#endif // USE_PSA_TEST_PARTITIONS

#ifdef USE_PSA_TEST_PARTITIONS
#define TFM_MAX_USER_PARTITIONS ({{service_partitions|count}} + {{test_partitions|count}})
#else
#define TFM_MAX_USER_PARTITIONS ({{service_partitions|count}})
#endif

#endif // __TFM_PARTITION_DEFS_INC__
