#!/bin/bash
# filosofo <id_filosofo> <num_filosofos> <ip_siguiente> <puerto_siguiente> \
# <puerto_local> <delay_conexion>
./filodist 0 5 127.0.0.1 40001 40000 15 &
./filodist 1 5 127.0.0.1 40002 40001 15 &
./filodist 2 5 127.0.0.1 40003 40002 15 &
./filodist 3 5 127.0.0.1 40004 40003 15 &
./filodist 4 5 127.0.0.1 40000 40004 15 &