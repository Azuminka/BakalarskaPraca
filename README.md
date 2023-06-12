# Container mechanisms in Linux OS
## Bachelor thesis

This repository contains source code in the C programming language aimed at demonstrating network namespace configuration. The repository also includes short examples of working with Docker. These codes have an educational purpose and aim to familiarize the reader with the functionalities offered by network namespaces and Docker.

Bachelor thesis: https://www.overleaf.com/read/ftcvvfdzzytd

### Manual

When running programs, it is necessary to use sudo.

Compilation: ` gcc program_name.c -o name `

Run: ` sudo ./name arguments `

| Program name          | Arguments                              | Run                                       | Directory                       |
| --------------------- | -------------------------------------- | ----------------------------------------- |-------------------------------- |
| create_namespace.c    |                                        | ./create_namespace                        | tvorba_menneho_priestoru                |
| move_device.c         | [namespace] [network_device]           | ./move_device ns eth0                     | pripojenie_sietoveho_zariadenia          |
| create_veth_pair.c    | [namespace1] [namespace2]              | ./create_veth_pair ns1 ns2                | veth_par                       |
| delete_veth_pair.c    | [namespace1] [namespace2]              | ./ delete_veth_pair ns1 ns2               | veth_par                       |
| migrate_namespace.c   |                                        | ./migrate_namespace                       | migracia_menneho?priestoru       |
| add_static_route.c    | [namespace] [route]                    | ./add_static_route ns 10.0.2.15/24        | sietove_smerovanie                         |
| delete_static_route.c | [namespace] [route]                    | ./delete_static_route ns 10.0.2.15/24     | sietove_smerovanie                         |
| show_routing_table.c  | [namespace]                            | ./show_routing_table ns                   | routing                         |
| set_ip.c              | [namespace] [ip_address] [mask]         | ./set_ip ns 10.0.2.15 24                  | IP_konfiguracia_podsiete         |
| set_primary_ip.c      | [namespace] [ip_address] [mask]         | ./set_primary_ip ns 10.0.2.15 24          | IP_konfiguracia_podsiete         |
| create_vlan.c         | [namespace] [VLAN_id]                  | ./create_vlan ns 2                        | vlan                            |
| configure_proxy.c     | [namespace] [proxy_server]             | ./configure_proxy ns http://1.1.1.1 8080  | proxy                           |
| communication_rules.c | [namespace]                            | ./communication_rules ns                  | firewall                        |
| ip_filtration.c       | [namespace] [ip_address]                | ./ip_filtration ns 10.0.2.15              | firewall                        |

### Useful commands:
Show all network namespaces: ` ip netns `

Execute the command within the network namespace: ` sudo ip netns exec namespace prikaz `

Add IP address to network interface: `ip addr add xxx.xxx.xxx.xxx/xx dev network_interface_name `

Activate network interface: `ifconfig network_interface_name up `

Move network interface from network namespace to host: `ip netns exec network_interface_name ip link set network_interface_name netns 1 `

Add route to routing table: `ip route add xxx.xxx.xxx.xxx/xx dev network_interface_name `





