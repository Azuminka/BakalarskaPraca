# Zakladny obraz na ktorom budeme stavat
FROM ubuntu:20.04

# Aktualizujeme system a nainstalujeme potrebne balicky
RUN apt-get update && apt-get install -y \
    cowsay \
    fortune

# Nastavime pracovny adresar
WORKDIR /usr/docker

# Nastavime ENTRYPOINT prikaz, ktory sa vykona po spusteni kontejnera
ENTRYPOINT ["/usr/docker/cowsay"]
