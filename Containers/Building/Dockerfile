FROM gcc:latest

COPY . ./ohceDockered

WORKDIR /ohceDockered

RUN gcc -o ohce reverse_server.c

ENV port=80

EXPOSE ${port}

#CMD ["./ohce", "${port}"]

CMD ./ohce ${port}