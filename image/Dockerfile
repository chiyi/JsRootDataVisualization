FROM rootproject/root:6.32.02-ubuntu24.04 as BASE

RUN apt update -y && apt install -y jq pip vim dos2unix
RUN pip install pandas --break-system-packages

RUN mkdir /sv_dir
WORKDIR /sv_dir
COPY scripts/dl_jsroot.sh /sv_dir
RUN /sv_dir/dl_jsroot.sh
COPY scripts/set_ssl.sh /etc/ssl/
RUN /etc/ssl/set_ssl.sh

FROM BASE as USER
RUN echo "USER PART"
ARG USERNAME
ARG GROUPNAME
ARG UID
ARG GID

RUN echo ${USERNAME}:${GROUPNAME} , ${UID}:${GID}
COPY scripts/set_user.sh /sv_dir
RUN /sv_dir/set_user.sh ${USERNAME}:${GROUPNAME} ${UID}:${GID}
RUN chmod -R 755 /sv_dir/jsroot
RUN chown -R ${UID}:${GID} /sv_dir/jsroot
