FROM rootproject/root:6.32.02-ubuntu24.04

RUN apt update -y && apt install -y jq

RUN mkdir /sv_dir
WORKDIR /sv_dir
COPY JsRoot772.tar.gz /sv_dir
RUN mkdir /sv_dir/jsroot
RUN tar xzvf JsRoot772.tar.gz -C jsroot
RUN chown -R 1000:1000 jsroot

