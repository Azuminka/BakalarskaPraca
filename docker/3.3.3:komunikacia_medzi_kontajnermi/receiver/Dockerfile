# Druhy kontajner

FROM ubuntu

RUN apt-get update && apt-get install -y cowsay netcat

COPY receive_message.sh /

CMD ["/bin/bash", "/receive_message.sh"]
