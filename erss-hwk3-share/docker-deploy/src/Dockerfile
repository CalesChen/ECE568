FROM ubuntu:18.04

RUN mkdir /temp
add . /temp
WORKDIR /temp
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update && apt-get install -y libpqxx-dev postgresql libxerces-c-dev

#ENTRYPOINT [ "./run.sh" ]