#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
#if DEVICE_EMAC
    #error [NOT_SUPPORTED] Not supported for WiFi targets
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"

#ifndef MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE 64
#endif

#ifndef MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT
#define MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT 500
#endif


namespace {
    char tx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    char rx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    const char ASCII_MAX = '~' - ' ';
    const int ECHO_LOOPS = 16;
}

void prep_buffer(char *uuid_buffer, size_t uuid_len, char *tx_buffer, size_t tx_size) {
    size_t i = 0;

    for (; i<uuid_len; ++i) {
        tx_buffer[i] = uuid_buffer[i];
    }

    tx_buffer[i++] = ' ';

    for (; i<tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}

int main() {
    char uuid[48] = {0};
    GREENTEA_SETUP_UUID(60, "udp_echo", uuid, 48);
    printf("Got a uuid of %s\r\n", uuid);
    size_t uuid_len = strlen(uuid);
    EthernetInterface eth;
    eth.connect();
    printf("UDP client IP Address is %s\n", eth.get_ip_address());

    greentea_send_kv("target_ip", eth.get_ip_address());

    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    UDPSocket sock;
    sock.open(&eth);
    sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);

    greentea_send_kv("host_ip", " ");
    greentea_parse_kv(recv_key, ipbuf, sizeof(recv_key), sizeof(ipbuf));

    greentea_send_kv("host_port", " ");
    greentea_parse_kv(recv_key, portbuf, sizeof(recv_key), sizeof(ipbuf));
    sscanf(portbuf, "%u", &port);

    printf("MBED: UDP Server IP address received: %s:%d \n", ipbuf, port);
    SocketAddress udp_addr(ipbuf, port);

    int success = 0;
    int i = 0;
    while (success < ECHO_LOOPS) {
        prep_buffer(uuid, uuid_len, tx_buffer, sizeof(tx_buffer));
        const int ret = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
        printf("[%02d] sent %d Bytes - %.*s  \n", i, ret, MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE, tx_buffer);
        SocketAddress temp_addr;
        const int n = sock.recvfrom(&temp_addr, rx_buffer, sizeof(rx_buffer));
        printf("[%02d] recv %d Bytes - %.*s  \n", i, ret, MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE, rx_buffer);

        if ((temp_addr == udp_addr &&
             n == sizeof(tx_buffer) &&
             memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer)) == 0)) {
            success += 1;

            printf("[%02d] success #%d\n", i, success);
        }
        i++;
    }

    bool result = success == ECHO_LOOPS;

    sock.close();
    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
