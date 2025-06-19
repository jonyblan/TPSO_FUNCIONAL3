# Limpia el proyecto a traves del container de Docker
docker start tpe_arqui_1q2024
docker exec -ti tpe_arqui_1q2024 make clean -C/root/Toolchain
docker exec -ti tpe_arqui_1q2024 make clean -C/root
docker stop tpe_arqui_1q2024