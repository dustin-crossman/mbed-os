#include "whd.h"
#include "whd_wifi_api.h"
#include "whd_network_types.h"
#include "whd_resource_api.h"


void host_network_process_ethernet_data( whd_interface_t ifp, /*@only@*/ whd_buffer_t buffer )
{
	//dummy function
}


whd_netif_funcs_t netif_ops =
{
    .whd_network_process_ethernet_data = host_network_process_ethernet_data,
};