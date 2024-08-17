FROM rootproject/root:6.32.02-ubuntu24.04 as BASE

RUN apt update -y && apt install -y jq

RUN mkdir /sv_dir
WORKDIR /sv_dir
COPY JsRoot772.tar.gz /sv_dir
RUN mkdir /sv_dir/jsroot
RUN tar xzvf JsRoot772.tar.gz -C jsroot
RUN chown -R 1000:1000 jsroot


FROM BASE as USER
RUN echo "USER PART"
ARG USERNAME
ARG GROUPNAME
ARG UID
ARG GID

RUN echo ${USERNAME}:${GROUPNAME} , ${UID}:${GID}
COPY set_user.sh /sv_dir
RUN /sv_dir/set_user.sh ${USERNAME}:${GROUPNAME} ${UID}:${GID}
RUN chmod 777 /sv_dir/jsroot
RUN chown -R ${UID}:${GID} jsroot

