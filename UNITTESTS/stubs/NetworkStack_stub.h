/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef NETWORKSTACKSTUB_H
#define NETWORKSTACKSTUB_H

#include "netsocket/NetworkStack.h"


class NetworkStackstub : public NetworkStack {
public:
    nsapi_error_t return_value = 0;
    virtual const char *get_ip_address()
    {
        return "127.0.0.1";
    }
    virtual nsapi_error_t gethostbyname(const char *host,
                                        SocketAddress *address, nsapi_version_t version)
    {
        return return_value;
    }
    virtual nsapi_value_or_error_t gethostbyname_async(const char *host, hostbyname_cb_t callback,
                                                       nsapi_version_t version)
    {
        return return_value;
    }
    virtual nsapi_error_t gethostbyname_async_cancel(int id)
    {
        return return_value;
    }

protected:
    virtual nsapi_error_t socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
    {
        return return_value;
    };
    virtual nsapi_error_t socket_close(nsapi_socket_t handle)
    {
        return return_value;
    };
    virtual nsapi_error_t socket_bind(nsapi_socket_t handle, const SocketAddress &address)
    {
        return return_value;
    };
    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog)
    {
        return return_value;
    };
    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address)
    {
        return return_value;
    };
    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address = 0)
    {
        return return_value;
    };
    virtual nsapi_size_or_error_t socket_send(nsapi_socket_t handle,
                                              const void *data, nsapi_size_t size)
    {
        return return_value;
    };
    virtual nsapi_size_or_error_t socket_recv(nsapi_socket_t handle,
                                              void *data, nsapi_size_t size)
    {
        return return_value;
    };
    virtual nsapi_size_or_error_t socket_sendto(nsapi_socket_t handle, const SocketAddress &address,
                                                const void *data, nsapi_size_t size)
    {
        return return_value;
    };
    virtual nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t handle, SocketAddress *address,
                                                  void *buffer, nsapi_size_t size)
    {
        return return_value;
    };
    virtual void socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data) {};

private:
    virtual nsapi_error_t call_in(int delay, mbed::Callback<void()> func)
    {
        return return_value;
    }
};

#endif // NETWORKSTACKSTUB_H
