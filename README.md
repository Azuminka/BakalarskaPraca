# Prehľad kontajnerizačných mechanizmov v OS Linux
## Bakalárska práca

Tento repozitár obsahuje zdrojové kódy v prgramovacom jazyku C, ktoré slúžia na demonštráciu konfigurácie v sieťových menných priestoroch. Repozitár taktiež obsahuje krátke ukážky k práci s Dockerom. Tieto kódy majú edukačný charakter a ich cieľom je oboznámiť čitateľa s funkcionalitami, aké so sebou prinášajú sieťové menné priestory a Docker.

Bakalárska práca: https://www.overleaf.com/read/ftcvvfdzzytd

### Manuál

Pri spúšťaní programov je nutné použiť sudo.

Kompilácia: ` gcc nazov_programu.c -o nazov `

Spustenie: ` sudo ./nazov argumenty `

| Názov programu        | Argumenty                              | Spustenie                                 | Adresár                         |
| --------------------- | -------------------------------------- | ----------------------------------------- |-------------------------------- |
| create_namespace.c    |                                        | ./create_namespace                        | tvorba_menneho_priestoru        |
| move_device.c         | [menny_priestor] [sietove_zariadenie]  | ./move_device ns eth0                     | pripojenie_sietoveho_zariadenia |
| create_veth_pair.c    | [menny_priestor1] [menny_priestor2]    | ./create_veth_pair ns1 ns2                | veth_par                        |
| delete_veth_pair.c    | [menny_priestor1] [menny_priestor2]    | ./ delete_veth_pair ns1 ns2               | veth_par                        |
| migrate_namespace.c   |                                        | ./migrate_namespace                       | migracia_menneho_priestoru      |
| add_static_route.c    | [menny_priestor] [trasa]               | ./add_static_route ns 10.0.2.15/24        | sietove_smerovanie              |
| delete_static_route.c | [menny_priestor] [trasa]               | ./delete_static_route ns 10.0.2.15/24     | sietove_smerovanie              |
| show_routing_table.c  | [menny_priestor]                       | ./show_routing_table ns                   | sietove_smerovanie              |
| set_ip.c              | [menny_priestor] [ip_adresa] [maska]   | ./set_ip ns 10.0.2.15 24                  | IP_konfiguracia_podsiete        |
| set_primary_ip.c      | [menny_priestor] [ip_adresa] [maska]   | ./set_primary_ip ns 10.0.2.15 24          | IP_konfiguracia_podsiete        |
| create_vlan.c         | [menny_priestor] [VLAN_id]             | ./create_vlan ns 2                        | vlan                            |
| configure_proxy.c     | [menny_priestor] [proxy_server]        | ./configure_proxy ns http://1.1.1.1 8080  | proxy                           |
| communication_rules.c | [menny_priestor]                       | ./communication_rules ns                  | firewall                        |
| ip_filtration.c       | [menny_priestor] [ip_adresa]           | ./ip_filtration ns 10.0.2.15              | firewall                        |

### Užitočné príkazy:
Zobrazí všetky sieťové menné priestory: ` ip netns `

Vykoná príkaz v kontexte sieťového menného priestoru: ` sudo ip netns exec nazov_menneho_priestoru prikaz `

Pridá IP adresu sieťovému rozhraniu: `ip addr add xxx.xxx.xxx.xxx/xx dev nazov_sietoveho_rozhrania `

Aktivuje sieťové rozhranie: `ifconfig nazov_sietoveho_rozhrania up `

Presunie sieťové rozhranie z menného priestoru na hostiteľský systém: `ip netns exec nazov_menneho_priestoru ip link set nazov_sietoveho_rozhrania netns 1 `

Pridá cestu do smerovacej tabuľky: `ip route add xxx.xxx.xxx.xxx/xx dev nazov_sietoveho_rozhrania `





