FROM mambaorg/micromamba:latest

COPY environment.yml .

RUN micromamba create -n poutre_env --file environment.yml && \
    micromamba clean --all --yes

ENV ENV_NAME=poutre_env

# Include project
ADD . /workspaces/poutre2
WORKDIR /workspaces/poutre2

CMD ["/bin/bash"]
